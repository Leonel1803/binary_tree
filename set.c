#include <stdlib.h>
#include <stdio.h>
#include "set.h"

/* Funciones para el manejo de los árboles */

int tree_insert(struct STRTNODE **root,TYPE e,COMPAREFUNC cf)
{
	int inserted = 0;
	if(*root==NULL)
	{
		*root = malloc(sizeof(struct STRTNODE));
		(*root)->elem = e;
		(*root)->left = NULL;
		(*root)->right = NULL;
		inserted = 1;
	}
	else if(cf(e,(*root)->elem)<0)  // e < (*root)->elem
		inserted = tree_insert(&(*root)->left,e,cf);
	else if(cf(e,(*root)->elem)>0)  // e > (*root)->elem
		inserted = tree_insert(&(*root)->right,e,cf);

	return inserted;
}


void tree_print(int level,struct STRTNODE *root,PRINTFUNC pf)
{
	int i;
	if(root!=NULL)
	{
		tree_print(level+1,root->right,pf);

		for(i=0;i<level;i++)
			printf("\t");
		pf(root->elem);

		tree_print(level+1,root->left,pf);
	}
}


SET set_create(COMPAREFUNC cf,PRINTFUNC pf)
{
	SET new_set = malloc(sizeof(struct STRSET));
	new_set->tree_root = NULL;	//Inicia con el árbol vacío
	new_set->size = 0;
	new_set->cf = cf;
	new_set->pf = pf;

	return new_set;
}

void set_add(SET set,TYPE e)
{
	if(tree_insert(&set->tree_root,e,set->cf))
		set->size = set->size + 1;
}

int set_size(SET set)
{
	return set->size;
}

void set_print(SET set)
{
	tree_print(0,set->tree_root,set->pf);
}

void set_remove(SET set, TYPE e) {
  struct STRTNODE *temp = set->tree_root;
  struct STRTNODE *padre = NULL;
  int el = *((int *)e);
  while (el != *(int *)temp->elem) {
    if (el < *((int *)temp->elem)) {
      padre = temp;
      temp = temp->left;
    } else if (el > *((int *)temp->elem)) {
      padre = temp;
      temp = temp->right;
    }
  }
  if (temp->left == NULL &&
      temp->right == NULL) { // Nodo a eliminar no tiene hijos
    if (padre == NULL) {     // Solo existe un solo nodo
      set->tree_root = NULL;
    } else if (temp == padre->left) { // Nodo a elminirar está a la izaquierda
                                      // de su padre
      padre->left = NULL;
    } else { // Nodo a elminirar está a la derecha de su padre
      padre->right = NULL;
    }
    free(temp);
  } else if (temp->left == NULL ||
             temp->right == NULL) { // Nodo a eliminar tiene un hijo
    struct STRTNODE *hijo;
    if (temp->left == NULL) { // Si el nodo a eliminar no tiene hijos a la
                              // izquierda, entonces tiene uno a la derecha
      hijo = temp->right;
    } else if (temp->right ==
               NULL) { // Si el nodo a eliminar no tiene hijos a la derecha,
                       // entonces tiene uno a la izquierda
      hijo = temp->left;
    }
    if (temp == padre->left) { // Se elimina temp e hijo lo reemplaza
      padre->left = hijo;
    } else { // Se elimina temp e hijo lo reemplaza
      padre->right = hijo;
    }
    free(temp);
  } else { // Nodo a eliminar tiene dos hijos
    // Encuentra el sucesor en orden del nodo a eliminar
    struct STRTNODE *sucesor = temp->right;
    if(sucesor->left != NULL){
      while (sucesor->left != NULL) {
        padre = sucesor;
        sucesor = sucesor->left;
      }
     // Reemplaza el valor del nodo a eliminar con el valor del sucesor
    temp->elem = sucesor->elem;
    // Elimina el sucesor en orden en su lugar original
    padre->left = NULL;
    free(sucesor);
    }
    else{
      temp->elem = sucesor->elem;
      temp->right = sucesor->right;
      free(sucesor);
    }
  }
  set->size--;
}