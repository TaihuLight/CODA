#ifndef core_PROTOCOL_PROTOCOL_H
#define core_PROTOCOL_PROTOCOL_H
#include "coda.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>
class Protocol {
public:
    Protocol(const std::string &description) : description(description) {}

    ~Protocol() {}

    std::string which() const { return description; }

    bool genKeypair(const std::string &metaPath) const;

    virtual bool encrypt(const std::string &inputFilePath,
                         const std::string &outputDirPath,
                         bool local_compute,
                         core::pk_ptr pk,
                         core::context_ptr context) const = 0;

    virtual bool decrypt(const std::string &inputFilePath,
                         const std::string &outputDirPath,
                         core::pk_ptr pk,
                         core::sk_ptr sk,
                         core::context_ptr context) const = 0;

    virtual bool evaluate(const std::vector<std::string> &inputDirs,
                          const std::string &outputDir,
                          core::pk_ptr pk,
                          core::context_ptr context) const = 0;

protected:
    virtual core::FHEArg parameters() const = 0;

private:
    std::string description;
};

namespace core {
static std::shared_ptr<Protocol> __currentProtocol = nullptr;
} // namespace core

namespace protocol {
bool genKeypair(core::Protocol protocol,
                std::ofstream &skStream,
                std::ofstream &ctxtStream,
                std::ofstream &pkStream);

namespace chi2 {
extern const core::FHEArg _fheArgs;

/// @param inputFilePath The raw data file.
/// @param outputDirPath The directory to place the cipher file(s).
/// @param pk The public encryption key.
bool encrypt(const std::string &inputFilePath,
             const std::string &outputDirPath,
             bool local_compute,
             core::pk_ptr pk,
             core::context_ptr context);

/// @param inputFilePath One cipher file.
/// @param outputFilePath The directory to place the result file.
/// @param pk The public encryption key.
/// @param sk The secret decryption key.
bool decrypt(const std::string &inputFilePath,
             const std::string &outputFilePath,
             core::pk_ptr pk,
             core::sk_ptr sk,
             core::context_ptr context);

/// @param inputDirs The collection of users' data directory.
///                  @see bool encrypt(const std::string &, const std::string &, core::pk_ptr).
/// @param outputDir The directory to place the evaluation result.
/// @param pk The public encryption key.
bool evaluate(const std::vector <std::string> &inputDirs,
              const std::string &outputDir,
              const std::vector<std::string> &params,
              core::pk_ptr pk,
              core::context_ptr context);
} // namespace chi2

namespace contingency {
extern const core::FHEArg _fheArgs;

bool encrypt(const std::string &inputFilePath,
             const std::string &outputDirPath,
             bool local_compute,
             core::pk_ptr pk,
             core::context_ptr context);

bool decrypt(const std::string &inputFilePath,
             const std::string &outputFilePath,
             core::pk_ptr pk,
             core::sk_ptr sk,
             core::context_ptr context);

bool evaluate(const std::vector <std::string> &inputDirs,
              const std::string &outputDir,
              const std::vector<std::string> &params,
              core::pk_ptr pk,
              core::context_ptr context);
} // contingency

namespace mean {
extern const core::FHEArg _fheArgs;

/// @param inputFilePath The raw data file.
/// @param outputDirPath The directory to place the cipher file(s).
/// @param pk The public encryption key.
bool encrypt(const std::string &inputFilePath,
             const std::string &outputDirPath,
             bool local_compute,
             core::pk_ptr pk,
             core::context_ptr context);

/// @param inputFilePath One cipher file.
/// @param outputFilePath The directory to place the result file.
/// @param pk The public encryption key.
/// @param sk The secret decryption key.
bool decrypt(const std::string &inputFilePath,
             const std::string &outputFilePath,
             core::pk_ptr pk,
             core::sk_ptr sk,
             core::context_ptr context);

/// @param inputDirs The collection of users' data directory.
///                  @see bool encrypt(const std::string &, const std::string &, core::pk_ptr).
/// @param outputDir The directory to place the evaluation result.
/// @param pk The public encryption key.
bool evaluate(const std::vector <std::string> &inputDirs,
              const std::string &outputDir,
              const std::vector<std::string> &params,
              core::pk_ptr pk,
              core::context_ptr context);
} // namespace mean

namespace rank {
extern const core::FHEArg _fheArgs;

/// @param inputFilePath The raw data file.
/// @param outputD
/// @param pk The public encryption key.
bool encrypt(const std::string &inputFilePath,
             const std::string &outputDirPath,
             bool local_compute,
             core::pk_ptr pk,
             core::context_ptr context);

/// @param inputFilePath One cipher file.
/// @param outputFilePath The directory to place the result file.
/// @param pk The public encryption key.
/// @param sk The secret decryption key.
bool decrypt(const std::string &inputFilePath,
             const std::string &outputFilePath,
             core::pk_ptr pk,
             core::sk_ptr sk,
             core::context_ptr context);

/// @param inputDirs The collection of users' data directory.
///                  @see bool encrypt(const std::string &, const std::string &, core::pk_ptr).
/// @param outputDir The directory to place the evaluation result.
/// @param pk The public encryption key.
bool evaluate(const std::vector<std::string> &inputDirs,
              const std::string &outputDir,
              const std::vector<std::string> &params,
              core::pk_ptr pk,
              core::context_ptr context);

} // namespace rank
} // namespace protocol
#endif // core_PROTOCOL_PROTOCOL_H
