/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CecWaiter.h"
#include "CecEnumParser.h"
#include "CecEnumChecker.h"
#include "CecCodeGenerator.h"
#include "cec.h"
#include "config.h"

#include <fstream>
#include <sstream>

using namespace cec;

int CecWaiter::work()
{
    int status = 1;

    if (!good())
    {
        return status;
    }

    // Input file to stream.
    auto stream = fileToStream(mFileInfo.sourceFile);
    if (stream.empty())
        return status;

    // Stream to enum class.
    auto pEnumClass = CecEnumParser::parse(stream);
    if (pEnumClass == nullptr)
    {
        return status;
    }

    // Basic check.
    if (!CecEnumChecker::check(*pEnumClass))
    {
        return status;
    }

    // Get sample stream file.
    std::string sampleStream;
    if(!mFileInfo.sampleFile.empty())
    {
        sampleStream = fileToStream(mFileInfo.sampleFile);
        if(sampleStream.empty())
        {
            return status;
        }
    }

    // Gen code and write file.
    auto codeStream = CecCodeGenerator::genCode(*pEnumClass, sampleStream);
    if(mFileInfo.targetFile.empty())
    {
        std::cout<< codeStream <<std::endl;
    }
    else
    {
        status = streamToFile(codeStream, mFileInfo.targetFile);
    }

    return status;
}

int CecWaiter::exportSample(const std::string &fileName)
{
    return streamToFile(config::CodeSample, fileName);
}

void CecWaiter::setSourceFile(const std::string &fileName)
{
    mFileInfo.sourceFile = fileName;
}

void CecWaiter::setTargetFile(const std::string &fileName)
{
    mFileInfo.targetFile = fileName;
}

void CecWaiter::setSampleFile(const std::string &fileName)
{
    mFileInfo.sampleFile = fileName;
}

std::string CecWaiter::fileToStream(const std::string &fileName)
{
    if (fileName.empty())
    {
        print_error("input file is empty.\n");
        return {};
    }

    std::ifstream ifs(fileName);
    if (!ifs.good())
    {
        print_error("cannot open file = ", fileName, "\n");
        ifs.close();
        return {};
    }

    ifs.seekg(0, ifs.end);
    if (ifs.tellg() > config::InputFileSizeMax)
    {
        print_error("input file is too big, max size allowed = ",
                    config::InputFileSizeMax, " bytes.\n");
        ifs.close();
        return {};
    }

    ifs.seekg(0, ifs.beg);
    std::istreambuf_iterator<char> begin(ifs);
    std::istreambuf_iterator<char> end;
    std::string stream(begin, end);
    ifs.close();

    if (stream.empty())
    {
        print_error("read empty file.\n");
    }

    return stream;
}

int CecWaiter::streamToFile(const std::string &stream, const std::string &fileName)
{
    int status = 0;

    if (stream.empty())
    {
        return -1;
    }

    if (fileName.empty())
    {
        return -1;
    }

    std::ofstream ofs(fileName);
    if (!ofs.good())
    {
        print_error("cannot write file = ", fileName, "\n");
        status = -1;
    }
    else
    {
        ofs << stream;
        ofs.flush();
        ofs.close();
    }
    return status;
}

bool CecWaiter::good()
{
    auto isSame = [](const std::string file1, const std::string &file2) -> int
    {
        return (!file1.empty() && !file2.empty() && file1 == file2);
    };

    auto printLine = [](const std::string &prefix, const std::string &file)
    {
        if (!file.empty())
        {
            print_error(prefix, file, "\n");
        }
    };

    int count = 0;
    count += isSame(mFileInfo.sourceFile, mFileInfo.targetFile);
    count += isSame(mFileInfo.targetFile, mFileInfo.sampleFile);
    count += isSame(mFileInfo.sampleFile, mFileInfo.sourceFile);
    if (count > 0)
    {
        print_error("Input file names cannot be the same,\n");
        printLine("  input-file-name      = ", mFileInfo.sourceFile);
        printLine("  enum-class-file-name = ", mFileInfo.targetFile);
        printLine("  unit-test-file-name  = ", mFileInfo.sampleFile);
        return false;
    }

    return true;
}