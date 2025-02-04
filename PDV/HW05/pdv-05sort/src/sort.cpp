#include "sort.h"

// implementace vaseho radiciho algoritmu. Detalnejsi popis zadani najdete v "sort.h"
void radix_par(std::vector<std::string*>& vector_to_sort, MappingFunction mapping_function,
               size_t alphabet_size, size_t str_size) {

    // sem prijde vase implementace. zakomentujte tuto radku
    //throw std::runtime_error("Not implemented.");
    std::vector<std::vector<std::string*>> buckets(alphabet_size, std::vector<std::string*>(0));
    std::vector<unsigned int> start(alphabet_size, 0);
    unsigned int string_size = vector_to_sort.at(0)->size();

    for(unsigned int i  = 0; i < vector_to_sort.size(); i++){
        std::string *str = vector_to_sort[i];
        unsigned int mix = mapping_function(str->at(string_size - str_size));
        buckets[mix].push_back(str);
    }

    if (str_size > 1){
        #pragma omp parallel for schedule(dynamic)
        for(unsigned int i = 0; i < alphabet_size; i++){
            if(buckets[i].size() > 1){
                std::vector<std::string *> &buckets_pointers = buckets[i];
                radix_par(buckets_pointers, mapping_function, alphabet_size, str_size-1);
            }
        }
    }
    for(unsigned int i = 1; i < alphabet_size; i++) { start[i] = start[i-1] + buckets[i-1].size(); }

    #pragma omp parallel for
    for (unsigned int i = 0; i < alphabet_size; i++){
        unsigned int offset = start[i];
        std::vector<std::string*> bucket = buckets[i];
        for(unsigned int j = 0; j < bucket.size(); j++){
            vector_to_sort[offset + j] = bucket[j];
        }
    }
    // abeceda se nemeni. jednotlive buckety by mely reprezentovat znaky teto abecedy. poradi znaku v abecede
    // dostanete volanim funkce mappingFunction nasledovne: mappingFunction((*p_retezec).at(poradi_znaku))

    // vytvorte si spravnou reprezentaci bucketu, kam budete retezce umistovat

    // pro vetsi jednoduchost uvazujte, ze vsechny retezce maji stejnou delku - string_lengths. nemusite tedy resit
    // zadne krajni pripady

    // na konci metody by melo byt zaruceno, ze vector pointeru - vector_to_sort bude spravne serazeny.
    // pointery budou serazeny podle retezcu, na ktere odkazuji, kdy retezcu jsou serazeny abecedne
}