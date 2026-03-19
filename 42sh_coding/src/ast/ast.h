#ifndef AST_H
#define AST_H

enum NodeType
{
    SIMPLE_COMMAND,
    COMMAND_LIST,
    IF_NODE,
    FOR_NODE,
    WHILE_NODE,
    UNTIL_NODE,
    OPERATORS,
    NEGATION,
    PIPELINE,
    REDIRECTION
};

enum type_redir
{
    IN,
    OUT
    //.......
};

enum operator_type
{
    OP_AND,
    OP_OR
};

struct AST_Node
{
    enum NodeType type;
    union
    {
        struct SimpleCommand
        {
            struct Linked_List *ll;
        } simple_command;
        /*struct Command
        {
            char **argv;
            struct redirection *redir;
        };*/
        struct CommandList
        {
            struct AST_Node *next;
            struct AST_Node *child;
        } command_list;
        struct IfNode
        {
            struct AST_Node *condition;
            struct AST_Node *then_branch;
            struct AST_Node *else_branch;
        } if_node;
        struct for_struct
        {
            char *var;
            struct Linked_List *values;
            struct AST_Node *to_do;
        } for_node;
        struct Opertors
        {
            enum operator_type op;
            struct AST_Node *left;
            struct AST_Node *right;
        } operators;
        struct loop_node
        {
            struct AST_Node *condition;
            struct AST_Node *to_do;
        } loop_node;
        struct negation
        {
            struct AST_Node *child;
        } negation;
        struct redirections
        {
            enum type_redir redir_type;
            char **target;
            struct AST_Node *next;
        } redirections;
    } structType;
};

void pretty_print_ast(struct AST_Node *ast);
void destroy_ast(struct AST_Node *ast);
struct AST_Node *create_ast_node(enum NodeType node);

#endif /* ! AST_H */
