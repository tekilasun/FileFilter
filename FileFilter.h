
#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include "boost/filesystem/operations.hpp"

namespace filesystem = boost::filesystem;

class FileInfo
{
public:

    FileInfo(const filesystem::path& path);

    inline const std::string& getFileName() const {return _name;}

    inline const std::string& getExtension() const {return _extension;}

    inline const size_t getSize() const {return _size;}

private:
    std::string _name;
    std::string _extension;
    size_t _size;
};

//////////////////////////////////////////////////////////////////////////
class Condition
{
public:
    virtual ~Condition(){}
    virtual bool check(const FileInfo& fileInfo) = 0;
};

class ConditionGroup: public Condition
{
public:
    virtual ~ConditionGroup() {}

    ConditionGroup& append(std::unique_ptr<Condition>&& condition)
    {
        _conditions.push_back(std::move(condition));
    }


    virtual bool check(const FileInfo& fileInfo) = 0;

protected:
    std::vector<std::unique_ptr<Condition>> _conditions;
};


class And: public ConditionGroup
{
public:
    virtual ~And() {}

    bool check(const FileInfo& fileInfo) override
    {
        for (auto it = _conditions.begin(); it != _conditions.end(); ++it)
        {
            if (!(*it)->check(fileInfo))
            {
                return false;
            }
        }
        return true;
    }
};

class Or: public ConditionGroup
{
public:
    virtual ~Or() {}

    bool check(const FileInfo& fileInfo) override
    {
        for (auto it = _conditions.begin(); it != _conditions.end(); ++it)
        {
            if ((*it)->check(fileInfo))
            {
                return true;
            }
        }
        return false;
    }

};

/**
 * Filter by extension
 */
class ExtensionsFilter: public Condition
{
public:
    ExtensionsFilter(const std::vector<std::string>& extensions):_extensions(extensions) {}

    virtual ~ExtensionsFilter() {}

protected:
    virtual bool check(const FileInfo &fileInfo) override;

private:
    std::vector<std::string> _extensions;
};

/**
 * Filter by size
 */

class SizeFilter: public Condition
{
public:

    static std::unique_ptr<Condition> createLessThan(size_t size);
    static std::unique_ptr<Condition> createGreaterThan(size_t size);
    static std::unique_ptr<Condition> createRangeFilter(size_t sizeMin, size_t sizeMax);

    virtual ~SizeFilter() {}

protected:

    SizeFilter(size_t size, std::function<bool(const size_t, const size_t)> func): _size(size), _func(func) {}
    virtual bool check(const FileInfo& fileInfo) override;

private:
    size_t _size;
    std::function<bool(const size_t, const size_t)> _func;
};

class FileFilter
{
public:
    typedef std::shared_ptr<FileFilter> PtrT;

public:

    FileFilter(): _conditions(new Or())
    {
    }

    virtual ~FileFilter() {}

    virtual bool accept(const FileInfo& fileInfo)
    {
        return  _conditions->check(fileInfo);
    }

    virtual void append(std::unique_ptr<Condition>&& nextElement)
    {
        _conditions->append(std::move(nextElement));
    }

private:
    std::unique_ptr<Or> _conditions;

};



