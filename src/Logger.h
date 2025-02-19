/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 urShadow
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

class Logger : public Singleton<Logger> {
public:
    void Init(logprintf_t logprintf, const std::string &prefix = "") {
        _logprintf = logprintf;
        _prefix = prefix;
    }

    template<typename ... ARGS>
    void Write(const std::string &fmt, ARGS ... args) {
        if (!_logprintf) {
            throw std::runtime_error{"logger was not initialized"};
        }

        _logprintf(("%s" + fmt).c_str(), _prefix.c_str(), args...);
    }

private:
    friend class Singleton<Logger>;

    Logger() : _logprintf{nullptr} {}

    logprintf_t _logprintf;
    std::string _prefix;
};

#endif // LOGGER_H_
