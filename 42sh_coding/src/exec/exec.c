#include "exec.h"
#include "../utils/utils.h"
#include "../expand/expand.h"

#include <complex.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int true_builtin(void)
{
	return 0;
}

int false_builtin(void)
{
	return 1;
}


static int exec_simple_command(struct expand_arguments *command, struct env_shell *env)
{
	
	if (strcmp(command->arg[0], "true") == 0)
	{
		return true_builtin();
	}
	else if (strcmp(command->arg[0], "false") == 0)
	{
		return false_builtin();
	}
	else if (strcmp(command->arg[0], "echo") == 0)
	{
		return echo(command->nb_arg - 1, command->arg + 1);
	}
	else if (strcmp(command->arg[0], "cd") == 0)
	{
		return cd(command->nb_arg - 1, command->arg + 1, env);
	}
	else if (strcmp(command->arg[0], "exit") == 0)
	{
		return exit_builtin(command->nb_arg - 1, command->arg + 1, env);
	}
	else
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			execvp(command->arg[0], &(command->arg[0]));
			exit(127);
		}
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			int code = WEXITSTATUS(status);
			return code;
		}
		return 2;
	}
}

static int exec_command_list(struct AST_Node *ast, struct env_shell *env)
{
	int err = 0;
	while (ast)
	{
		err = exec_ast(ast->structType.command_list.child, env);
		ast = ast->structType.command_list.next;
	}

	return err;
}


static int exec_if_node(struct AST_Node *ast, struct env_shell *env)
{
	int err = 0;
	err = exec_ast(ast->structType.if_node.condition, env);
	if (err == 0)
	{
		ast = ast->structType.if_node.then_branch;
		return exec_ast(ast, env);	
	}
	else if (err == 1)
	{
		if (!ast->structType.if_node.else_branch)
		{
			return 0;
		}
		ast = ast->structType.if_node.else_branch;
		return exec_ast(ast, env);
	}
	else
	{
		return 2;
	}
}

static int exec_operators(struct AST_Node *ast, struct env_shell *env)
{
    int left_status = exec_ast(ast->structType.operators.left, env);

    if (ast->structType.operators.op == OP_AND)
    {
        if (left_status == 0)
        {
            return exec_ast(ast->structType.operators.right, env);
        }
        return left_status;
    }
    if (left_status != 0)
    {
        return exec_ast(ast->structType.operators.right, env);
    }
    return left_status;
}

static int exec_negation(struct AST_Node *ast, struct env_shell *env)
{
	return !exec_ast(ast->structType.negation.child, env);;
}

int first_pipe(struct AST_Node *ast, int *fds, struct list_pid *list, struct env_shell *env)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[0]);
		close(fds[1]);
		exit(exec_ast(ast, env));
	}
	add_pid(list, pid);
	return 0;
}
static int middle_pipe(struct AST_Node *ast, int *fds, struct list_pid *list, int *err, struct env_shell *env)
{
	int lecture = fds[0];
	close(fds[1]);
	*err = pipe(fds);
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		dup2(lecture, STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		exit(exec_ast(ast, env));
	}
	add_pid(list, pid);
	close(lecture);
	return 0;
}
static int last_pipe(struct AST_Node *ast, int *fds, struct list_pid *list, struct env_shell *env)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		exit(exec_ast(ast, env));
	}
	add_pid(list, pid);
	return 0;
}



static int exec_pipeline(struct AST_Node *ast, struct env_shell *env)
{
	struct AST_Node *next = ast->structType.command_list.next;
	struct list_pid *list = init_list_pid();
	int err;
	int fds[2];
	if (!next)
	{
		err = wait_all_and_free(list);
		return exec_ast(ast->structType.command_list.child, env);
	}
	else 
	{
		err = pipe(fds);
		first_pipe(ast->structType.command_list.child, fds, list, env);
		ast = next;
	}

	while (ast->structType.command_list.next) 
	{
		middle_pipe(ast->structType.command_list.child, fds, list, &err, env);
		ast = ast->structType.command_list.next;
	}
	last_pipe(ast->structType.command_list.child, fds, list, env);
	close(fds[0]);
	close(fds[1]);
	err = wait_all_and_free(list);
	return err;
}

int exec_ast(struct AST_Node *ast, struct env_shell *env)
{
	if (!ast)
    {
        return 0;
    }
	
	if (ast->type == OPERATORS)
    {
        return exec_operators(ast, env);
    }
	if (ast->type == SIMPLE_COMMAND)
	{
		int *err = malloc(sizeof(int));
		struct expand_arguments *command = convert_ll_to_char(ast, err, env);
		if (command->nb_arg == 0 || *err != 0)
		{ 
			free_command(command);
			int code = *err;
			free(err);
			return code;
		}
		free(err);
		int res = exec_simple_command(command, env);
		free_command(command);
		return res;
	}
	else if (ast->type == COMMAND_LIST)
	{
		return exec_command_list(ast, env);
	}
	else if (ast->type == NEGATION) 
	{
		return exec_negation(ast, env);
	}
	else if (ast->type == PIPELINE) 
	{
		return exec_pipeline(ast, env);
	}
	else
	{
		return exec_if_node(ast, env);
	}
}
