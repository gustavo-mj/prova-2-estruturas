// (1) poda da árvore enraizada em 'data':
template<typename T>
void BinaryTree<T>::pruning(T data) {
    if (empty()) return;
    Node* node = root_->acha(data);
    if (node == nullptr) return;

    // Se é a raiz, apagamos a árvore toda
    if (node == root_) {
        delete root_;
        root_ = nullptr;
        size_ = 0;
        return;
    }

    // Senão, apagamos a subárvore
    Node* parent = node->parent_;
    if (parent->left_ == node) parent->left_ = nullptr;
    else parent->right_ = nullptr;
    std::size_t old_size = size_;
    ArrayList<T> v;
    node->percorre(v);
    delete node;
    size_ -= v.size();
}

// (2) criação de uma árvore binária de busca balanceada da união
//     da própria com outra B passada por parâmetro:
template<typename T>
BinaryTree<T> BinaryTree<T>::balanced_union(BinaryTree<T> *B) {
    ArrayList<T> listaA = this->in_order();
    ArrayList<T> listaB = B->in_order();
    ArrayList<T> uniao{listaA.size() + listaB.size()};

    std::size_t i = 0, j = 0;
    while (i < listaA.size() && j < listaB.size()) {
        if (listaA[i] < listaB[j]) {
            uniao.push_back(listaA[i++]);
        } else if (listaB[j] < listaA[i]) {
            uniao.push_back(listaB[j++]);
        } else {
            uniao.push_back(listaA[i++]);
            j++;
        }
    }
    while (i < listaA.size()) uniao.push_back(listaA[i++]);
    while (j < listaB.size()) uniao.push_back(listaB[j++]);

    BinaryTree<T> nova;
    busca_binaria(nova, uniao, 0, uniao.size() - 1);
    return nova;
}

// (3) criação de uma lista contendo o comprimento de caminho de
//     cada folha até a raiz (na sequência "em ordem" de folhas):
template<typename T>
ArrayList<int> BinaryTree<T>::leaves_path_length() {
    ArrayList<Node*> folhas;
    ArrayList<int> comprimentos;
    if (!empty()) {
        root_->acha_folhas(folhas);
        for (std::size_t i = 0; i < folhas.size(); ++i) {
            Node* atual = folhas[i];
            int profundidade = 0;
            while (atual->parent_ != nullptr) {
                atual = atual->parent_;
                profundidade++;
            }
            comprimentos.push_back(profundidade);
        }
    }
    return comprimentos;
}

// (4) remoção dos nós, da própria árvore, pertencentes ao maior
//     comprimento de caminho, e retorno da lista de valores da
//     folha até a raiz:
template<typename T>
ArrayList<T> BinaryTree<T>::remove_max_path() {
    ArrayList<T> resultado;
    if (empty()) return resultado;

    ArrayList<Node*> folhas;
    root_->acha_folhas(folhas);

    Node* folha_max = nullptr;
    int max_len = -1;

    for (std::size_t i = 0; i < folhas.size(); ++i) {
        Node* atual = folhas[i];
        int len = 0;
        Node* aux = atual;
        while (aux->parent_ != nullptr) {
            len++;
            aux = aux->parent_;
        }
        if (len > max_len) {
            max_len = len;
            folha_max = atual;
        }
    }

    Node* atual = folha_max;
    while (atual != nullptr) {
        resultado.push_back(atual->data_);
        Node* pai = atual->parent_;
        if (pai != nullptr) {
            if (pai->left_ == atual) pai->left_ = nullptr;
            else pai->right_ = nullptr;
        } else {
            root_ = nullptr;
        }
        Node* temp = atual;
        atual = pai;
        delete temp;
        size_--;
    }

    return resultado;
}
