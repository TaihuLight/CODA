//
// Created by riku on 2016/07/28.
//
#include "HElib/Ctxt.h"
#include "HElib/EncryptedArray.h"
#include "core/ctxt_util.hpp"
#include "core/coda.hpp"
#include "core/contingency_table.hpp"
#include "core/greaterthan.h"
#include <vector>
#include <algorithm>

namespace core {
size_t PrivateContingencyTableHelper::repeats_per_cipher() const {
    auto cont_table_size = block_size();
    assert(cont_table_size <= ea->size() &&
           "One cipher can't contain the whole contingency table");
    return ea->size() / cont_table_size;
}

size_t PrivateContingencyTableHelper::how_many_copies(long domain_size) const {
    size_t repeats = repeats_per_cipher();
    return (domain_size + repeats - 1UL) / repeats;
}

size_t PrivateContingencyTableHelper::
how_many_copies_for_bits(const EncryptedArray *ea) const {
    auto bit_per = (number_bits(ea->getAlMod().getPPowR()) / 8UL) << 3;
    printf("%ld %ld\n", ea->getAlMod().getPPowR(),
           number_bits(ea->getAlMod().getPPowR()));
    return (aesKeyLength() + bit_per - 1) / bit_per;
}

size_t PrivateContingencyTableHelper::block_size() const {
    auto modified_size = coprime(P.size, Q.size);
    return modified_size.first * modified_size.second;
}

std::shared_ptr <Ctxt> PrivateContingencyTableHelper::repeat(size_t R) const {
    assert(CT != nullptr && "the CT haven't been set");
    size_t repeats = std::min(R, repeats_per_cipher());
    if (repeats != R)
        printf("Warning! hope to repeat %zd times but only get %zd\n", R,
               repeats);
    return std::make_shared<Ctxt>(
            core::repeat0(*CT, block_size(), repeats, *ea));
}

void PrivateContingencyTableHelper::
open_gamma(std::vector <Publishable> &unsuppression,
           const Type_gamma &gamma,
           const Type_tilde_gamma &tilde_gamma,
           const EncryptedArray *ea, sk_ptr sk) const {
    size_t bs = block_size();
    size_t usable_size = ea->size() / bs * bs;
    auto modified_sizes = coprime(P.size, Q.size);
    auto bit_per = number_bits(ea->getAlMod().getPPowR());
    std::vector<long> decrypted(ea->size());

    for (size_t i = 0; i < gamma.size(); i++) {
        const auto &part = gamma.at(i);
        if (!part->isCorrect())
            printf("Warnning! might be an invalid cipher\n");
        ea->decrypt(*part, *sk, decrypted);
        std::vector <size_t> zeros;
        for (size_t j = 0; j < usable_size; j++) {
            if (decrypted.at(j) != 0) continue;
            zeros.push_back(j);
        }

        if (zeros.empty()) continue;

        auto aesKeys = decryptToGetAESKeys(tilde_gamma, i, zeros, ea, sk);

        for (size_t j = 0; j < zeros.size(); j++) {
            size_t u = zeros[j] % modified_sizes.first;
            size_t v = zeros[j] % modified_sizes.second;
            Publishable info = {.u = u, .v = v,
                                .aes_key = convKey(aesKeys.at(j), bit_per)};
            unsuppression.push_back(info);
        }
    }
}

std::vector <PrivateContingencyTable::AESKey_t>
PrivateContingencyTableHelper::decryptToGetAESKeys(
        const Type_tilde_gamma &tilde_gammas,
        const size_t loc,
        const std::vector <size_t> &zeros,
        const EncryptedArray *ea, sk_ptr sk) const {
    size_t bs = block_size();
    std::vector <PrivateContingencyTable::AESKey_t> keys(zeros.size());
    size_t usable_size = ea->size() / bs * bs;
    std::vector<long> slots;

    for (auto &parts : tilde_gammas) {
        ea->decrypt(*(parts.at(loc)), *sk, slots);
        for (size_t j = 0; j < zeros.size(); j++) {
            keys.at(j).push_back(slots.at(zeros[j]));
        }
    }
    return keys;
}


std::vector<long> PrivateContingencyTableHelper::
final_decrypt(const Type_n_uv &cells,
              const std::vector<struct PrivateContingencyTableHelper::Publishable> &publishable,
              const sk_ptr sk, const EncryptedArray *ea) const {
    assert(cells.size() >= publishable.size() && "Mismatch size!");
    std::vector<long> plain_cells(cells.size(), 0);

    const FHEPubKey &pk = *sk;
    Ctxt ctxt(pk);
    std::vector<long> slots(ea->size(), 0);
    ea->encrypt(ctxt, pk, slots);
    for (auto &p : publishable) {
        std::cout << "Publishable Key " << p.aes_key << "\n";
        AES128 aes(p.aes_key);
        auto idx = p.u * p.v;
        std::string decrypt_aes = aes.decrypt(cells.at(idx));

        //conv(ctxt, decrypt_aes);
        ea->decrypt(ctxt, *sk, slots);

        long count = 0;
        for (auto s : slots) {
            if (s != 0) {
                assert(count != 0);
                count = s;
            }
        }

//        assert(count > 0);
        plain_cells.push_back(count);
    }

    return plain_cells;
}

} // namespace core