// Directory utilities. This library contains functions for locating configuration directories, for
// testing if a command with a given name can be found in the PATH, and various other path-related
// issues.
#ifndef FISH_PATH_H
#define FISH_PATH_H

#include <string>

#include "common.h"
#include "env.h"
#include "maybe.h"
#include "parser.h"
#include "wutil.h"

/// Returns the user configuration directory for fish. If the directory or one of its parents
/// doesn't exist, they are first created.
///
/// \param path The directory as an out param
/// \return whether the directory was returned successfully
bool path_get_config(wcstring &path);

/// Returns the user data directory for fish. If the directory or one of its parents doesn't exist,
/// they are first created.
///
/// Volatile files presumed to be local to the machine, such as the fish_history and all the
/// generated_completions, will be stored in this directory.
///
/// \param path The directory as an out param
/// \return whether the directory was returned successfully
bool path_get_data(wcstring &path);

enum class dir_remoteness_t {
    unknown,  // directory status is unknown
    local,    // directory is known local
    remote,   // directory is known remote
};

/// Finds the path of an executable named \p cmd, by looking in $PATH taken from \p vars.
/// On success, err will be 0 and the path is returned.
/// On failure, we return the "best path" with err set appropriately.
/// For example, if we find a non-executable file, we will return its path and EACCESS.
/// If no candidate path is found, path will be empty and err will be set to ENOENT.
/// Possible err values are taken from access().
struct get_path_result_t {
    int err;
    wcstring path;
};
get_path_result_t path_try_get_path(const wcstring &cmd, const environment_t &vars);

/// Returns the full path of the specified directory, using the CDPATH variable as a list of base
/// directories for relative paths.
///
/// If no valid path is found, false is returned and errno is set to ENOTDIR if at least one such
/// path was found, but it did not point to a directory, or ENOENT if no file of the specified
/// name was found.
///
/// \param dir The name of the directory.
/// \param wd The working directory. The working directory must end with a slash.
/// \param vars The environment variables to use (for the CDPATH variable)
/// \return the command, or none() if it could not be found.
maybe_t<wcstring> path_get_cdpath(const wcstring &dir, const wcstring &wd,
                                  // todo!("should be environment_t")
                                  const env_stack_t &vars);

/// Returns the given directory with all CDPATH components applied.
std::vector<wcstring> path_apply_cdpath(const wcstring &dir, const wcstring &wd,
                                        const env_stack_t &env_vars);

/// Check if two paths are equivalent, which means to ignore runs of multiple slashes (or trailing
/// slashes).
bool paths_are_equivalent(const wcstring &p1, const wcstring &p2);

/// Appends a path component, with a / if necessary.
void append_path_component(wcstring &path, const wcstring &component);

#endif
