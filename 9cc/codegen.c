#include "9cc.h"

void gen_lval(Node *node) {
    if (node->ty == ND_IDENT) {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", ('z' - node->name + 1) * 8);
        printf("  push rax\n");
        return;
    } else {
        fprintf(stderr, "代入の左辺値が変数ではありません\n");
        exit(1);
    }
}

void gen(Node *node) {
    if (node->ty == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    } else if (node->ty == ND_IDENT) {
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    } else if (node->ty == '=') {
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    } else {
        gen(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");

        switch (node->ty) {
            case '+':
                printf("  add rax, rdi\n");
                break;
            case '-':
                printf("  sub rax, rdi\n");
                break;
            case '*':
                printf("  mul rdi\n");
                break;
            case '/':
                printf("  mov rdx, 0\n");
                printf("  div rdi\n");
        }

        printf("  push rax\n");
    }
}