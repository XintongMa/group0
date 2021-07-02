#ifndef TESTS_USERPROG_KERNEL_TESTS_H
#define TESTS_USERPROG_KERNEL_TESTS_H

void run_test(const char*);

typedef void test_func(void);

extern test_func test_fp_kasm;
extern test_func test_fp_kinit;

void msg(const char*, ...);
void fail(const char*, ...);
void pass(void);

#endif /* tests/userprog/kernel/tests.h */
