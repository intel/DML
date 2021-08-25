/*
 * Copyright 2020-2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

/**
 * @brief Contains a runner for Intel DML Job API Examples
 * @date 1/21/2020
 *
 */

#include "stdio.h"

#include "dml.h"
#include "dml_examples.h"

/* ------  HELPER FUNCTIONS  ------ */

/**
 * @brief Displays an Intel logo and legals
 */
void print_hello()
{
    printf("Intel(R) Data Mover Library Job API Examples\n\n"
           "============================== LEGALS ==============================\n"
           "\n"
           "Copyright 2020-2021 Intel Corporation\n"
           "\n"
           "This software and the related documents are Intel copyrighted materials,\n"
           "and your use of them is governed by the express license under which they\n"
           "were provided to you (\"License\"). Unless the License provides otherwise,\n"
           "you may not use, modify, copy, publish, distribute, disclose or transmit\n"
           "this software or the related documents without Intel's prior written\n"
           "permission.\n"
           "\n"
           "This software and the related documents are provided as is, with no\n"
           "express or implied warranties, other than those that are expressly\n"
           "stated in the License.\n"
           "====================================================================\n");
}

/* ------  MAIN FUNCTION  ------ */

/**
 * @brief Just main function to direct call of examples
 */
int main(const int argc, char **const argv)
{
    // Variables
    dml_job_t *dml_job_ptr = NULL;
    uint32_t total_fails = 0u;

    // Allocate dml_job_t
    dml_job_ptr = init_dml_job(DML_PATH_SW);

    dml_status_t (*sample_function)(dml_job_t *const);

    print_hello();

    // Run each sample
    for (uint32_t sample = 0u; sample < SAMPLES_COUNT; sample++)
    {
        dml_status_t status = 0u;

        printf("\n------------------------------------------\n");
        printf("\tRun example # %d\n", sample + 1u);

        sample_function = wrapper_launchers[sample];

        status = sample_function(dml_job_ptr);

        if (status)
        {
            total_fails++;
        }

        printf("\tExample return: %s (Status %d)\n", (status) ? "FAIL" : "OK", status);
    }

    // Print examples execution results
    printf("\n====== Examples Execution Completed ======\n");
    printf("\t --- Total Samples run:                     %d\n", SAMPLES_COUNT);
    printf("\t --- Samples completed with OK status:      %d\n", SAMPLES_COUNT - total_fails);
    printf("\t --- Samples completed with FAIL status:    %d\n", total_fails);

    // Free memory
    finalize_dml_job(dml_job_ptr);

    return total_fails;
}
