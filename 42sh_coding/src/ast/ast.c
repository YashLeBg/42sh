#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"

static struct AST_Node *create_simple_command(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    return res;
}

static struct AST_Node *create_command_list(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.command_list.child = NULL;
    res->structType.command_list.next = NULL;
    return res;
}

static struct AST_Node *create_if_node(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.if_node.condition = NULL;
    res->structType.if_node.then_branch = NULL;
    res->structType.if_node.else_branch = NULL;
    return res;
}

static struct AST_Node *create_for_node(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.for_node.var = NULL;
    res->structType.for_node.values = NULL;
    res->structType.for_node.to_do = NULL;
    return res;
}

static struct AST_Node *create_loop_node(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.loop_node.condition = NULL;
    res->structType.loop_node.to_do = NULL;
    return res;
}

static struct AST_Node *create_operators(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.operators.left = NULL;
    res->structType.operators.right = NULL;
    return res;
}

static struct AST_Node *create_negation(void)
{
    struct AST_Node *res = malloc(sizeof(struct AST_Node));
    if (!res)
    {
        return NULL;
    }
    res->structType.negation.child = NULL;
    return res;
}

struct AST_Node *create_ast_node(enum NodeType node)
{
    struct AST_Node *res;
    if (node == SIMPLE_COMMAND)
    {
        res = create_simple_command();
    }
    else if (node == OPERATORS)
    {
        res = create_operators();
    }
    else if (node == COMMAND_LIST || node == PIPELINE)
    {
        res = create_command_list();
    }
    else if (node == IF_NODE)
    {
        res = create_if_node();
    }
    else if (node == FOR_NODE)
    {
        res = create_for_node();
    }
    else if (node == WHILE_NODE || node == UNTIL_NODE)
    {
        res = create_loop_node();
    }
    else
    {
        res = create_negation();
    }
    res->type = node;
    return res;
}

void pretty_print_ast(struct AST_Node *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->type == SIMPLE_COMMAND)
    {
        printf("->SIMPLE_COMMAND\n");
    }
    else if (ast->type == COMMAND_LIST)
    {
        printf("START COMMAND_LIST\n");
        pretty_print_ast(ast->structType.command_list.child);
        pretty_print_ast(ast->structType.command_list.next);
        printf("END COMMAND_LIST\n");
    }
    else if (ast->type == IF_NODE)
    {
        printf("IF\n");
        pretty_print_ast(ast->structType.if_node.condition);
        printf("THEN\n");
        pretty_print_ast(ast->structType.if_node.then_branch);
        if (ast->structType.if_node.else_branch)
        {
            printf("ELSE\n");
            pretty_print_ast(ast->structType.if_node.else_branch);
        }
        printf("FI\n->");
    }
    else if (ast->type == PIPELINE)
    {
        pretty_print_ast(ast->structType.command_list.child);
        printf("PIPELINE\n");
        pretty_print_ast(ast->structType.command_list.next);
    }
}

void destroy_ast(struct AST_Node *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->type == SIMPLE_COMMAND)
    {
        free_ll(ast->structType.simple_command.ll);
        free(ast);
    }
    else if (ast->type == COMMAND_LIST || ast->type == PIPELINE)
    {
        destroy_ast(ast->structType.command_list.child);
        destroy_ast(ast->structType.command_list.next);
        free(ast);
    }
    else if (ast->type == IF_NODE)
    {
        destroy_ast(ast->structType.if_node.condition);
        destroy_ast(ast->structType.if_node.then_branch);
        destroy_ast(ast->structType.if_node.else_branch);
        free(ast);
    }
    else if (ast->type == OPERATORS)
    {
        destroy_ast(ast->structType.operators.left);
        destroy_ast(ast->structType.operators.right);
        free(ast);
        return;
    }
}
