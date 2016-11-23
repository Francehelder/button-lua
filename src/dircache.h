/**
 * Copyright (c) Jason White
 *
 * MIT License
 *
 * Description:
 * A cache for the contents of a directory. This also handles reporting
 * directory dependencies.
 */
#pragma once

#include <map>
#include <string>
#include <vector>

#include "path.h"

class ImplicitDeps;

struct DirEntry {
    std::string name;
    bool isDir;

    // Implement ordering for sorting purposes.
    friend bool operator<(const DirEntry& a, const DirEntry& b);
};

typedef std::vector<DirEntry> DirEntries;

typedef void (*GlobCallback)(Path path, bool isDir, void* data);

/**
 * A cache for directory listings.
 */
class DirCache {
private:
    // Mapping of directory names to directory contents.
    std::map<std::string, DirEntries> cache;

    ImplicitDeps* _deps;

public:
    DirCache(ImplicitDeps* deps = nullptr);

    /**
     * Returns a list of names in the given directory.
     */
    const DirEntries& dirEntries(const std::string& path);

    /**
     * Convenience function. The two paths are joined and then looked up.
     */
    const DirEntries& dirEntries(Path root, Path dir);

    /**
     * Globs for files starting at the given root.
     *
     * Parameters:
     *   root     = The root directory to start searching from. All matched
     *              paths are relative to this directory.
     *   path     = The path which can contain glob patterns. Recursive glob
     *              expressions are also supported.
     *   callback = The function to call for every matched file name.
     *   data     = Data to pass along to the callback function.
     */
    void glob(Path root, Path path, GlobCallback callback,
            void* data = nullptr);

    /**
     * Helper function. |path| must not contain a glob pattern.
     */
    void glob(Path root, Path path, Path pattern, GlobCallback callback,
            void* data = nullptr);

private:

    void globRecursive(Path root, std::string& path, GlobCallback callback,
            void* data = nullptr);
};