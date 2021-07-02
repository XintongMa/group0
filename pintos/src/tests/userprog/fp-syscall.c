/* Pushes values to the FPU, then jumps into the kernel to perform a
   system call (which uses floating point arithmetic).
   Ensures that floating point registers are saved on a system call. */

#include <float.h>
#include <syscall-nr.h>
#include "tests/lib.h"
#include "tests/main.h"

#define NUM_VALUES 8
const char* test_name = "fp-syscall";
static int values[NUM_VALUES] = {1, 6, 2, 162, 126, 2, 6, 1};

/* Invokes syscall NUMBER, passing argument ARG0, and returns the
   return value as an `int'. */
#define syscall1(NUMBER, ARG0)                                                                     \
  ({                                                                                               \
    int retval;                                                                                    \
    asm volatile("pushl %[arg0]; pushl %[number]; int $0x30; addl $8, %%esp"                       \
                 : "=a"(retval)                                                                    \
                 : [number] "i"(NUMBER), [arg0] "g"(ARG0)                                          \
                 : "memory");                                                                      \
    retval;                                                                                        \
  })

void test_main(void) {
  msg("Computing e...");
  push_values_to_fpu(values, NUM_VALUES);

  // Manually call the system call so that the compiler does not
  // generate FP instructions that modify the FPU in user space
  int e_res = syscall1(SYS_COMPUTE_E, 10);
  if (pop_values_from_fpu(values, NUM_VALUES)) {
    msg("Success!");
  } else {
    msg("Incorrect values popped");
    exit(126);
  }

  // Convert the integer returned by the system call into a proper
  // double, so we can see if the return value is correct
  float e_res_flt;
  ASSERT(sizeof(float) == sizeof(int));
  memcpy(&e_res_flt, &e_res, sizeof(int));
  double e_res_dbl = (double) e_res_flt;
  if (abs(e_res_dbl - E_VAL) < TOL) {
    msg("Kernel computation successful");
    exit(162);
  } else {
    msg("Got e=%f, expected e=%f", e_res_dbl, E_VAL);
    exit(261);
  }
}
