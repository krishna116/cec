/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include <string>
#include <memory>

namespace cec
{
    class CecWaiter
    {
    public:
        int work();
        int exportSample(const std::string &fileName);
        
        void setSourceFile(const std::string &fileName);
        void setTargetFile(const std::string &fileName);
        void setSampleFile(const std::string &fileName);
    private:
    	struct FileInfo
        {
            std::string sourceFile;
            std::string targetFile;
            std::string sampleFile;
        };

        FileInfo mFileInfo;

        /**
         * @brief Read file to stream.
         * 
         * @param fileName          File to be read.
         * 
         * @return std::string      Stream.
         */
        std::string fileToStream(const std::string &fileName);
        /**
         * @brief Save stream to file.
         * 
         * @param stream        Stream to be saved. 
         * @param fileName      File to be write.
         * 
         * @retval 0            pass.
         * @retval other        fail. 
         */
        int streamToFile(const std::string &stream, const std::string &fileName);

        bool good();
    };
} // namespace cec
