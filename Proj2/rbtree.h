#ifndef __RBTREE_H__
/**
 * Árvore Rubro-Negra
 * ------------------
 *
 * A maioria das operações abaixo retorna um booleano para
 * indicar erro na operação. Elas retornam `false` (0)
 * quando não ocorrer um erro e `true`, (valor não-zero)
 * para indicar algum problema.
 *
 * No evento de um erro, a árvore não é alterada e outros
 * ponteiros passados para a função não são sobrescritos.
 *
 * Funções sem retorno não podem gerar erros, dado que todos
 * os ponteiros, inclusive a árvore, são válidos.
 */
#define __RBTREE_H__

#include "utils.h"


// Tipo das chaves da árvore.
typedef uint32_t chave_t;

/**
 *  Estrutura da Árvore Rubro-Negra.
 */
struct rbtree {
    struct node *raiz;
};


/**
 *  Inicialização de árvore vazia.
 */
struct rbtree rb_nova(void)
attribute(const, cold, leaf, nothrow);

/**
 *  Desalocação da árvore.
 */
void rb_dealloc(struct rbtree *tree)
attribute(nonnull, cold, leaf, nothrow);

/**
 *  Inserção da chave na árvore.
 *
 *  Retorna se um dos seguintes erros ocorreu:
 *   - erro alocação de memória.
 */
bool rb_insere(struct rbtree *arvore, chave_t chave)
attribute(nonnull, leaf, cold, nothrow, access(read_write, 1));

/**
 *  Busca o sucessor em-ordem de uma chave.
 *  O resultado será escrito em `succ`.
 *
 *  Retorna se um dos seguintes erros ocorreu:
 *   - chave não está presente na árvore.
 *   - chave não tem sucessor.
 */
bool rb_busca_succ(const struct rbtree *arvore, chave_t chave, chave_t *succ)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 3));

/**
 *  Busca o predecessor em-ordem de uma chave.
 *  O resultado será escrito em `pred`.
 *
 *  Retorna se um dos seguintes erros ocorreu:
 *   - chave não está presente na árvore.
 *   - chave não tem predecessor.
 */
bool rb_busca_pred(const struct rbtree *arvore, chave_t chave, chave_t *pred)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 3));

/**
 *  Busca o máximo da árvore.
 *  O resultado será escrito em `max`.
 *
 *  Retorna se um dos seguintes erros ocorreu:
 *   - árvore não tem máximo (árvore vazia).
 */
bool rb_busca_max(const struct rbtree *arvore, chave_t *max)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 2));

/**
 *  Busca o mínimo da árvore.
 *  O resultado será escrito em `min`.
 *
 *  Retorna se um dos seguintes erros ocorreu:
 *   - árvore não tem mínimo (árvore vazia).
 */
bool rb_busca_min(const struct rbtree *arvore, chave_t *min)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 2));

/**
 *  Aplica uma função em cada chave da árvore,
 *  com percurso pré-ordem.
 */
void rb_pre_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave))
attribute(nonnull, cold, access(read_only, 1));

/**
 *  Aplica uma função em cada chave da árvore,
 *  com percurso em-ordem.
 */
void rb_em_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave))
attribute(nonnull, cold, access(read_only, 1));

/**
 *  Aplica uma função em cada chave da árvore,
 *  com percurso pós-ordem.
 */
void rb_pos_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave))
attribute(nonnull, cold, access(read_only, 1));

#endif //__RBTREE_H__
