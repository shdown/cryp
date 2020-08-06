#pragma once

#include <system_error>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

class CFile
{
    FILE *file_;

public:
    CFile(const char *path, const char *mode)
    {
        if (!(file_ = fopen(path, mode))) {
            const int saved_errno = errno;
            throw std::system_error(saved_errno, std::generic_category(), path);
        }
    }

    CFile(const CFile &) = delete;

    CFile(CFile &&other)
        : file_{other.file_}
    {
        other.file_ = nullptr;
    }

    CFile & operator =(const CFile &) = delete;

    CFile & operator =(CFile &&other)
    {
        std::swap(file_, other.file_);
        return *this;
    }

    operator FILE * () { return file_; }

    ~CFile()
    {
        if (file_) {
            fclose(file_);
        }
    }
};

class CFileLine
{
    char *buf_;
    size_t nbuf_;
public:
    CFileLine()
        : buf_{nullptr}
        , nbuf_{0}
    {}

    CFileLine(const CFileLine &) = delete;

    CFileLine(CFileLine &&other)
        : buf_{other.buf_}
        , nbuf_{other.nbuf_}
    {
        other.buf_ = nullptr;
    }

    CFileLine& operator =(const CFileLine &) = delete;

    CFileLine& operator =(CFileLine &&other)
    {
        std::swap(buf_, other.buf_);
        std::swap(nbuf_, other.nbuf_);
        return *this;
    }

    ssize_t read_from(FILE *f)
    {
        return getline(&buf_, &nbuf_, f);
    }

    char * c_str()
    {
        assert(buf_ && nbuf_);
        return buf_;
    }

    ~CFileLine()
    {
        free(buf_);
    }
};
