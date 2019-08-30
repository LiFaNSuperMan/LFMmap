//
//  mmap_util.c
//
//  Created by 李建伟 on 2019/6/27.
//  Copyright © 2019 李建伟. All rights reserved.
//

#include "mmap_util.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "mmap_status.h"

#define PRINT_ERROR printf("Error mmap_util %d : error\n",__LINE__ )

static size_t default_file_size =  1024 * 1024;

/**
 内存不够时的更新策略

 @param neededSize 需要设置的大小
 @return 扩容之后的大小
 */
static inline size_t allocationSizeWithNeededSize(size_t neededSize) {
    size_t allocationSize = (neededSize >> 3) + (neededSize < 9 ? 3 : 6);
    size_t res = (allocationSize + neededSize) > default_file_size ? allocationSize + neededSize : default_file_size;

    return res;
}

MMAP_FILE * open_mmapfile(const char * __restrict __filepath , int *error){
    
    int _fd = open(__filepath, O_RDWR | O_CREAT, 0666);
    if (_fd == 0) {
        PRINT_ERROR;
        *error = MMAP_ERROR_FD_INIT;
        return NULL;
    }
    struct stat statInfo;
    if(fstat(_fd, &statInfo) !=0 ){
        PRINT_ERROR;
        *error = MMAP_ERROR_FD_INIT;
        return NULL;
    };
    size_t m_size = statInfo.st_size > default_file_size ?: default_file_size;
    void *_mmptr = mmap(NULL, m_size , PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, _fd, 0);
    if (_mmptr == (void *)-1) {
        PRINT_ERROR;
        *error = MMAP_ERROR_MMAP_MEMORY;
        return NULL;
    }
    ftruncate(_fd,m_size);
    
    MMAP_FILE *model = (MMAP_FILE *)malloc(sizeof(MMAP_FILE));
    model->file_path = __filepath;
    model->fd = _fd;
    model->mmaptr = _mmptr;
    model->file_size = m_size;

    return model;
    

}

int write_mmapfile(MMAP_FILE *model, const void * __restrict __ptr ,long start, size_t len , bool permit_dilatation){
    
    if (len + start > model->file_size) {
        if (permit_dilatation) {
            munmap(model->mmaptr, model->file_size);
            size_t needed_size = allocationSizeWithNeededSize(len + start);
            void *mmptr = mmap(NULL, needed_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, model->fd, 0);
            ftruncate(model->fd, needed_size);
            model->file_size = needed_size;
            model->mmaptr = mmptr;
        }else{
            // 内存不够
            return MMAP_ERROR_MEMORY;
        }
    }
    memcpy(model->mmaptr + start, __ptr, len);
    
    return MMAP_STATUS_SUCCESS;

}

int close_mmapfile(MMAP_FILE *model){
    
    if (model) {
        if (model->mmaptr) {
            munmap(model->mmaptr, model->file_size);
        }
        if (model->fd) {
            ftruncate(model->fd, model->file_size);
            close(model->fd);
        }
        free(model);
        model = NULL;
    }
    return MMAP_STATUS_SUCCESS;
}

int restroe_mmapfile(MMAP_FILE *model){
    
    munmap(model->mmaptr, model->file_size);
    void *mmptr = mmap(NULL, default_file_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, model->fd, 0);
    ftruncate(model->fd, default_file_size);
    memset(model->mmaptr, 0, default_file_size);
    model->file_size = default_file_size;
    model->mmaptr = mmptr;

    return 1;
}
