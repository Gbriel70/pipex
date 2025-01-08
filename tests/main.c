#include "minunit.h"
#include "../include/pipex.h"
#include <stdio.h>


char *exec_cmd(char *cmd)
{
    int line_len;
    FILE    *stream;
    char    *output;

    line_len = 0;
    output = ft_calloc(1000, sizeof(char));
    stream = popen(cmd, "r");
    if (stream == NULL)
    {
        printf("popen failed to execute.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (fgets(output + line_len, 1000, stream))
        {
            line_len += strlen(output + line_len);
        }
    }
    pclose(stream);
    return (output);
}

MU_TEST(function_should_run_comand_ls_l_wc_l)
{
    char   *expected = "Files ../outfile and ../outfile_expected are identical\n";
    char    *outfile;
    char    *diff_result;

    printf("\n------------------------\n");
    printf(" TEST 1: ls -l | wc -l");
    printf("\n------------------------\n");

    exec_cmd("< ../infile ls -l | wc -l > ../outfile_expected");
    exec_cmd("../pipex ../infile \"ls -l\" \"wc -l\" ../outfile");

    diff_result = exec_cmd("LC_ALL=C diff -s ../outfile ../outfile_expected");
    ft_printf("%s", diff_result);
    outfile = exec_cmd("cat ../outfile");
    ft_printf("Outfile: %s", outfile);
    mu_assert_string_eq(expected, diff_result);
    free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_run_command_grep_a1_wc_w)
{
	char    *expected = "Files ../outfile2 and ../outfile_expected2 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 2: grep a1 | wc -w");
	printf("\n------------------------\n");

	exec_cmd("< ../infile grep a1 | wc -w > ../outfile_expected2");
	exec_cmd("../pipex ../infile \"grep a1\" \"wc -w\" ../outfile2");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile2 ../outfile_expected2");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile2");
	ft_printf("Outfile: %s", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_run_command_cat_ls_l)
{
	char    *expected = "Files ../outfile3 and ../outfile_expected3 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 3: cat | ls -l");
	printf("\n------------------------\n");

	exec_cmd("< ../infile cat | ls -l > ../outfile_expected3");
	exec_cmd("../pipex ../infile \"cat\" \"ls -l\" ../outfile3");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile3 ../outfile_expected3");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile3");
	ft_printf("Outfile: %s", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_run_command_cat_wc)
{
	char    *expected = "Files ../outfile4 and ../outfile_expected4 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 4: cat | wc");
	printf("\n------------------------\n");

	exec_cmd("< ../infile cat | wc > ../outfile_expected4");
	exec_cmd("../pipex ../infile \"cat\" \"wc\" ../outfile4");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile4 ../outfile_expected4");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile4");
	ft_printf("Outfile: %s", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_run_command_echo_tr)
{
	char    *expected = "Files ../outfile5 and ../outfile_expected5 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 5: echo | tr -d");
	printf("\n------------------------\n");

	exec_cmd("< ../infile echo CAIU_NA_VILA_O_PEIXE_FUZILA| tr -d _ > ../outfile_expected5");
	exec_cmd("../pipex ../infile \"echo CAIU_NA_VILA_O_PEIXE_FUZILA\" \"tr -d _\" ../outfile5");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile5 ../outfile_expected5");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile5");
	ft_printf("Outfile: %s", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_create_outfile_and_run_second_command)
{
	char    *expected = "Files ../outfile6 and ../outfile_expected6 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 6: invalid infile");
	printf("\n------------------------\n");

	exec_cmd("< ../file_x cat | ls > ../outfile_expected6");
	exec_cmd("../pipex ../file_x \"cat\" \"ls\" ../outfile6");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile6 ../outfile_expected6");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile6");
	ft_printf("Pipex outfile: %s\n", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(funtion_should_run_command_echo_tr_space)
{
	char    *expected = "Files ../outfile7 and ../outfile_expected7 are identical\n";
	char    *outfile;
	char    *diff_result;

	printf("\n------------------------\n");
	printf(" TEST 7: echo | tr space");
	printf("\n------------------------\n");

	exec_cmd("< ../infile echo \"h e l l o\" | tr -d ' ' > ../outfile_expected7");
	exec_cmd("../pipex ../infile \"echo \'h e l l o\'\" \"tr -d ' '\" ../outfile7");

	diff_result = exec_cmd("LC_ALL=C diff -s ../outfile7 ../outfile_expected7");
	ft_printf("%s", diff_result);
	outfile = exec_cmd("cat ../outfile7");
	ft_printf("Pipex outfile: %s\n", outfile);
	mu_assert_string_eq(expected, diff_result);
	free(diff_result);
	free(outfile);
}

MU_TEST(function_should_try_run_command_without_x_permission)
{
    char *expected = "Files ../outfile8 and ../outfile_expected8 are identical\n";
    char *outfile;
    char *diff_result;

    printf("\n------------------------\n");
    printf(" TEST 8 echo | tr space: ");
    printf("\n------------------------\n");

    exec_cmd("< ../infile grep pipex | /dev/null > ../outfile_expected8");
    exec_cmd("../pipex ../infile \"grep pipex\" \"/dev/null\" ../outfile8");

    diff_result = exec_cmd("LC_ALL=C diff -s ../outfile8 ../outfile_expected8");
    ft_printf("%s", diff_result);
    outfile = exec_cmd("cat ../outfile8");
    ft_printf("Pipex outfile: %s\n", outfile);
    mu_assert_string_eq(expected, diff_result);
    free(diff_result);
	free(outfile);
}

MU_TEST(function_should_try_run_command_with_path)
{
    char *expected = "Files ../outfile9 and ../outfile_expected9 are identical\n";
    char *outfile;
    char *diff_result;

    printf("\n------------------------\n");
    printf(" TEST 9 first command with path: ");
    printf("\n------------------------\n");

    exec_cmd("< ../infile ////////usr/bin/grep 1 | wc -l > ../outfile_expected9");
    exec_cmd("../pipex ../infile \"////////usr/bin/grep 1\" \"wc -l\" ../outfile9");

    diff_result = exec_cmd("LC_ALL=C diff -s ../outfile9 ../outfile_expected9");
    ft_printf("%s", diff_result);
    outfile = exec_cmd("cat ../outfile9");
    ft_printf("Pipex outfile: %s\n", outfile);
    mu_assert_string_eq(expected, diff_result);
    free(diff_result);
	free(outfile);
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(function_should_run_comand_ls_l_wc_l);
    MU_RUN_TEST(funtion_should_run_command_grep_a1_wc_w);
    MU_RUN_TEST(funtion_should_run_command_cat_ls_l);
    MU_RUN_TEST(funtion_should_run_command_cat_wc);
    MU_RUN_TEST(funtion_should_run_command_echo_tr);
    MU_RUN_TEST(funtion_should_create_outfile_and_run_second_command);
    MU_RUN_TEST(funtion_should_run_command_echo_tr_space);
    MU_RUN_TEST(function_should_try_run_command_without_x_permission);
    MU_RUN_TEST(function_should_try_run_command_with_path);
}

int main()
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}