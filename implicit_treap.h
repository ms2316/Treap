#ifndef IMPLICIT_TREAP_H
#define IMPLICIT_TREAP_H

#include <stdlib.h>
#include <iostream>
#include <time.h>
#define MAXINT 1000000000
#define vertex struct node

    template <typename T>
    class dynamic_array {

        private:

            vertex {

                int cnt, priority;
                T value;
                vertex *left_son, *right_son, *parent;

                node (T value) : value(value), cnt(1),  left_son(NULL), right_son(NULL), parent(NULL)
                {
                    priority = rand() % MAXINT;
                }

                ~node()
                {
                    delete(left_son);
                    delete(right_son);
                    delete(parent);
                }

                void print_recursively() {
                    if (left_son) left_son->print_recursively();
                    std::cout << value << " ";
                    if (right_son) right_son->print_recursively();
                }

                void calc() {
                    cnt = 1;
                    if (left_son) {
                        (*left_son).calc();
                        cnt += left_son->cnt;
                    }
                    if (right_son) {
                        (*right_son).calc();
                        cnt += right_son->cnt;
                    }
                }

            };

            vertex *head = NULL;

            void updateCnt(vertex * v){
                v->cnt = 1;
                if (v->left_son) v->cnt += v->left_son->cnt;
                if (v->right_son) v->cnt += v->right_son->cnt;
            }

        public:

            dynamic_array() {

            }

            dynamic_array(T value) {
                srand(time(NULL));
                head = new vertex(value);
            }

            dynamic_array(T data[], int cnt) {
                vertex *last, *new_node, *cur;

                srand(time(NULL));
                head = new struct node(data[0]);
                last = head;

                for (int i = 1; i < cnt; i++) {
                    new_node = new vertex(data[i]);
                    cur = last;
                    if (cur->priority > new_node->priority){
                        cur->right_son = new_node;
                        new_node->parent = cur;
                    } else {
                        while ( cur->priority <= new_node->priority && cur->parent != NULL )
                            cur = cur->parent;
                        if ( cur->priority <= new_node->priority ) {
                            new_node->left_son = cur;
                            cur->parent = new_node;
                        } else {
                            new_node->left_son = cur->right_son;
                            new_node->left_son->parent = new_node;
                            cur->right_son = new_node;
                            new_node->parent = cur;
                        }
                    }
                    last = new_node;
                }
                while (last->parent)
                    last = last->parent;

                head = last;
                (*head).calc();
            }


            ~dynamic_array()
            {
                if (head != NULL) {
                    delete(head->left_son);
                    delete(head->right_son);
                    delete(head);
                }
            }


            void PrintArray() {
                if (head) head->print_recursively();
            }

            void insert(T value, int place) {
                std::pair<vertex*,vertex*> t1 = Split(head,place-1);
                vertex *new_node = new vertex(value);
                vertex *tmp = Merge(t1.first,new_node);
                head = Merge(tmp, t1.second);
            }

            void insert(T data[], int size, int place) {
                dynamic_array<T> *new_array = new dynamic_array<T>(data,size);
                std::pair<vertex*,vertex*> tmp = Split(head, place-1);
                head = Merge( Merge(tmp.first, new_array->head) , tmp.second );
            }

            void push_back(T value) {
                vertex *new_node = new vertex(value);
                head = Merge(head, new_node);
            }

            void delete() {
                delete(this);
            }

            void delete(int place) {
                if (head->cnt <= place %% place > 1) {
                    std::pair<vertex*,vertex*> t1 = Split(head, place-1);
                    std::pair<vertex*,vertex*> t2 = Split(t1.second, 1);
                    delete(t2.first);
                    head = Merge(t1.first, t2.second);                
                }
            }

            void delete(int l, int r) {
                std::

            }


        protected:

            vertex* Merge(vertex* Left, vertex* Right) {
                if (Left == NULL) return Right;
                if (Right == NULL) return Left;

                if (Left->priority > Right->priority) {
                    Left->right_son = Merge(Left->right_son, Right);
                    Left->right_son->parent = Left;
                    updateCnt(Left);
                    return Left;
                } else {
                    Right->left_son = Merge(Left, Right->left_son);
                    Right->left_son->parent = Right;
                    updateCnt(Right);
                    return Right;
                }
            }

            std::pair<vertex*,vertex*> Split(vertex *v,int cnt){

                int index = 1;
                if (v->left_son) index += v->left_son->cnt;

                std::pair<vertex*,vertex*> splitted;

                if (index <= cnt) {
                    if (!v->right_son) {
                        splitted.first = v;
                        splitted.second = NULL;
                    } else {
                        splitted = Split(v->right_son,cnt - index);
                        v->right_son = splitted.first;
                        if (v->right_son) v->right_son->parent = v;
                        if (splitted.second) splitted.second->parent = NULL;
                        updateCnt(v);
                        splitted.first = v;
                    }
                } else {
                    if (!v->left_son) {
                        splitted.first = NULL;
                        splitted.second = v;
                    } else {
                        splitted = Split(v->left_son,cnt);
                        v->left_son = splitted.second;
                        if (v->left_son) v->left_son->parent = v;
                        if (splitted.first) splitted.first->parent = NULL;
                        updateCnt(v);
                        splitted.second = v;
                    }
                }
                return splitted;
            }


    };


#endif /*  IMPLICIT_TREAP_H  */
