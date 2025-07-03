template<typename T>
void BinaryTree<T>::pruning(T data) {
    if (empty()) return;

    typename BinaryTree<T>::Node* sub_root = root_->acha(data);
    if (sub_root == nullptr) return;

    // Remove subárvore enraizada em sub_root
    if (sub_root->parent_ == nullptr) {
        delete root_;
        root_ = nullptr;
        size_ = 0;
    } else {
        if (sub_root->parent_->left_ == sub_root)
            sub_root->parent_->left_ = nullptr;
        else
            sub_root->parent_->right_ = nullptr;
        std::size_t before = size_;
        delete sub_root;

        // Recontar elementos após a poda
        ArrayList<T> v;
        if (root_ != nullptr)
            root_->percorre(v);
        size_ = v.size();
    }
}

template<typename T>
BinaryTree<T> BinaryTree<T>::balanced_union(BinaryTree<T> *B) {
    BinaryTree<T> t;
    if (!empty()) {
        ArrayList<T> vA{size_};
        root_->in_order(vA);
        for (std::size_t i = 0; i < vA.size(); ++i)
            t.insert(vA[i]);
    }
    if (B != nullptr && !B->empty()) {
        ArrayList<T> vB{B->size()};
        B->root_->in_order(vB);
        for (std::size_t i = 0; i < vB.size(); ++i)
            t.insert(vB[i]);
    }

    // Ordenar e criar árvore balanceada
    ArrayList<T> lista_ordenada = t.in_order();
    BinaryTree<T> result;
    busca_binaria(result, lista_ordenada, 0, lista_ordenada.size() - 1);
    return result;
}

template<typename T>
ArrayList<int> BinaryTree<T>::leaves_path_length() {
    ArrayList<int> r;

    if (empty()) return r;

    ArrayList<typename BinaryTree<T>::Node*> folhas;
    root_->acha_folhas(folhas);

    for (std::size_t i = 0; i < folhas.size(); ++i) {
        typename BinaryTree<T>::Node* atual = folhas[i];
        int altura = 0;
        while (atual->parent_ != nullptr) {
            atual = atual->parent_;
            ++altura;
        }
        r.push_back(altura);
    }

    return r;
}

template<typename T>
ArrayList<T> BinaryTree<T>::remove_max_path() {
    ArrayList<T> r;
    if (empty()) return r;

    // Encontra folha mais profunda
    ArrayList<typename BinaryTree<T>::Node*> folhas;
    root_->acha_folhas(folhas);

    typename BinaryTree<T>::Node* folha_mais_funda = nullptr;
    int max_depth = -1;

    for (std::size_t i = 0; i < folhas.size(); ++i) {
        typename BinaryTree<T>::Node* atual = folhas[i];
        int depth = 0;
        while (atual->parent_ != nullptr) {
            atual = atual->parent_;
            ++depth;
        }
        if (depth > max_depth) {
            max_depth = depth;
            folha_mais_funda = folhas[i];
        }
    }

    // Remove todos os nós do caminho folha -> raiz
    typename BinaryTree<T>::Node* atual = folha_mais_funda;
    while (atual != nullptr) {
        r.push_back(atual->data_);
        typename BinaryTree<T>::Node* pai = atual->parent_;
        remove(atual->data_);
        atual = pai;
    }

    return r;
}
