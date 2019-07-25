//
//  mmap_util.h
//  FireflyStasistics
//
//  Created by 李建伟 on 2019/6/27.
//  Copyright © 2019 李建伟. All rights reserved.
//

#ifndef mmap_util_h
#define mmap_util_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// unsafe   


typedef struct {
    size_t file_size;
    const char * __restrict file_path;
    int fd ;
    void * mmaptr;
} MMAP_FILE;


/**
 打开一个文件

 @param __filepath 文件路径
 @return 本地MMAP文件结构体模型
 */

MMAP_FILE * open_mmapfile(const char * __restrict __filepath , int *error);

/**
 写入一个内容到某个文件

 @param model 本地MMAP文件结构体模型
 @param __ptr 文件content
 @param start 开始写入的位置
 @param len 写入的长度
 @param permit_dilatation 当写入长度大于文件长度时 是否允许扩容
 @return status
 */
int write_mmapfile(MMAP_FILE *model, const void * __restrict __ptr ,long start, size_t len , bool permit_dilatation);
/**
 关闭

 @param model 本地MMAP文件结构体模型
 @return status
 */
int close_mmapfile(MMAP_FILE *model);
/**
 重置本地文件

 @param model 本地数据模型
 @return status
 */
int restroe_mmapfile(MMAP_FILE *model);

#endif /* mmap_util_h */
