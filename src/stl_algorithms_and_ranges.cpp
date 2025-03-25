namespace functional {
void main();
}
namespace ranges_and_views {
void main();
}
namespace swaps {
void main();
}
namespace compare {
void main();
}
namespace sorting {
void main();
}
namespace partitioning {
void main();
}
namespace divide_and_conquer {
void main();
}
namespace linear_operations {
void main();
}
namespace set_operations {
void main();
}
namespace transformation {
void main();
}
namespace left_folds {
void main();
}
namespace general_reductions {
void main();
}
namespace boolean_reductions {
void main();
}
namespace generators {
void main();
}
namespace copy_and_move {
void main();
}
namespace search_and_compare {
void main();
}
namespace min_max_algorithms {
void main();
}
namespace heap_data_structure {
void main();
}

int main() {
    // A biblioteca de algoritmos encontrada na stl vem sendo continuadamente
    // trabalhada a medida que as novas standards vêm sendo lançadas (c++98,
    // c++11, c++14, c++17, c++20 e c++23).
    // Se de um lado a stl provém estruturas de dados e containeres para
    // diversas situações (array, vector, queue, list, map, set, etc), por outro
    // também provém diversos algoritmos padronizados para realizar suas
    // operações de forma eficaz sobre essas estruturas de dados.
    // Ainda, a stl faz extensivo uso de iteradores e ranges que servem como
    // adaptadores e permitem a abstração e o desacoplamento entre os algoritmos
    // e as estruturas de dados.
    // Pode-se pensar num iterador como um objeto, que se comporta de forma
    // similar à um ponteiro. Este possui métodos para operações de deferência
    // (*) para aquisição do valor apontado e, a depender de sua categoria,
    // também sabe realizar a iteração ao longo da sua respectiva estrutura de
    // dados por meio de métodos como 'begin', 'end', 'next', 'prev', '+', '++',
    // etc.
    // Uma range é composta por, num primeiro momento, dois iteradores,
    // definindo o primeiro elemento e a posição após o último elemento,
    // respectivamente. pode-se representar uma range por [begin, end). Por meio
    // das standards mais recentes, os tipos dos iteradores que definem uma
    // range não precisam ser do mesmo tipo. em geral, o iterador que define o
    // fim também é chamado de 'sentinela' e pode representar até mesmo um valor
    // inalcançável como std::unreachable_sentinel, para denotar uma range
    // infinita.
    // As categorias de iteradores e ranges são:
    // - input/output iterator: lê e escreve cada elemento exatamente uma vez.
    // - forward iterator: lê e escreve cada elemento repetidamente e avança
    //                     unidirecionalmente.
    // - bidirectional iterator: forward iterator + movimentação bidirecional
    //                           (capaz de retroagir).
    // - random access iterator: bidirecional iterator + capaz de avançar ou
    //                           retornar por meio de um número 'n' inteiro e de
    //                           calcular a distância entre dois iteradores.
    // - contiguous iterator: random access iterator + o armazenamento dos
    //                        elementos é feito de forma contínua na memória
    //                        (caso de 'array', 'vector', etc).

    ranges_and_views::main();
    // functional::main();
    // swaps::main();
    // compare::main();
    // sorting::main();
    // partitioning::main();
    // divide_and_conquer::main();
    // linear_operations::main();
    // set_operations::main();
    // transformation::main();
    // left_folds::main();
    // general_reductions::main();
    // boolean_reductions::main();
    // generators::main();
    // copy_and_move::main();
    // search_and_compare::main();
    // min_max_algorithms::main();
    // heap_data_structure::main();
};
