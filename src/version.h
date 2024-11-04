#ifndef VERSION_H
#define VERSION_H

/*
    These macros will be automatically overwritten by cmake
    Changes shouldn't be checking into the repo

    To ignore this file:
    git update-index --skip-worktree src/version.h

    To un-ignore this file:
    git update-index --no-skip-worktree src/version.h
*/

// clang-format off
#define VERSION_MAJOR @raytracer_VERSION_MAJOR@
#define VERSION_MINOR @raytracer_VERSION_MINOR@
// clang-format on

#endif
