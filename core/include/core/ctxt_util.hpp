//
// Created by riku on 2016/07/19.
//

#ifndef CODA_CLION_CTXT_UTIL_HPP
#define CODA_CLION_CTXT_UTIL_HPP
#include <cstddef>
#include <vector>
#include <NTL/ZZX.h>
#include "greaterthan.h"

class Ctxt;
class EncryptedArray;
namespace core {
/// repeat the first n_slots of 'c' for 'rep' times
/// For example, c = [1, 2, 3, 4]; then repeat(c, 3, 2) => [1, 2, 3, 1, 2 3]
Ctxt repeat(const Ctxt &c, const long n_slots, const long rep, const EncryptedArray &ea);
/// For slots except the first n_slots are zeros
Ctxt repeat0(const Ctxt &c, const long n_slots, const long rep, const EncryptedArray &ea);

void mask_first(Ctxt &ctxt, size_t a, const EncryptedArray &ea);

std::vector<std::vector<long>> random_permutation(long D, const EncryptedArray &ea);

std::vector<std::vector<long>> randomness(long D, const EncryptedArray &ea);

size_t number_bits(long a);
} // namespace core
#endif //CODA_CLION_CTXT_UTIL_HPP
