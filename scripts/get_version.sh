#!/bin/bash

#This script should be run only from the Makefile
#It generates version.h

GITV_H=inc/version.h
GITV_H_TMP=inc/version.h.tmp
mkdir -p "inc"

GIT_RAW_VERSION=$(git describe --always --dirty --long --tags)
TOTAL_NUM_COMMITS=$(git rev-list --count HEAD)
BRANCH=$(git rev-parse --abbrev-ref HEAD)
AHEAD_BY_MASTER=$(git log --oneline master..${BRANCH} | wc -l | bc)
TOTAL_NUM_UNTRACKED=$(git ls-files --exclude-standard --others --full-name -- . | wc -l | bc)
COMMIT_HASH=$(git rev-parse --short --verify HEAD)

IFS='-' read -r -a git_raw_version <<< "$GIT_RAW_VERSION"
IFS='v' read -r -a tag <<< "${git_raw_version[0]}"
IFS='.' read -r -a major_minor <<< "${tag[1]}"

MAJOR=${major_minor[0]}
MINOR=${major_minor[1]}
BUILD=${git_raw_version[1]}
VERSION="$MAJOR.$MINOR.$BUILD"

GIT_TIMESTAMP=$(git log -1 --pretty=format:'%ci' --date=short --abbrev-commit)
IFS=' ' read -r -a timestamp <<< "${GIT_TIMESTAMP}"
COMMIT_DATE=${timestamp[0]}
COMMIT_TIME=${timestamp[1]}
COMMIT_TIMEZONE=${timestamp[2]}

COMPILE_TIME=$(date +%H:%M:%S)
COMPILE_DATE=$(date +%Y-%m-%d)

#Generate temporary header file
git log -1 --pretty=format:'/******************************************************************************
	About
******************************************************************************/

/**
* \file version.h
*
* \brief	This file is generated automatically from script and keeps track of version control.
*
* \author	Ilias Kanelis	hkanelhs@yahoo.gr
*/

/**
* \defgroup	Version	Version
*
* \code	#include <version.h>	@endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef GIT_VERSION_H_ONLY_ONE_INCLUDE_SAFETY
#define GIT_VERSION_H_ONLY_ONE_INCLUDE_SAFETY

/************************************************
	Custom definitions
************************************************/

/**
 * Raw git version.
 */
#define GIT_RAW_VERSION					"'$GIT_RAW_VERSION'"

/**
 * Version of the project.
 */
#define VERSION							"'$VERSION'"

/**
 * Major version from git tag.
 */
#define MAJOR							"'$MAJOR'"

/**
 * Minor version from git tag
 */
#define MINOR							"'$MINOR'"

/**
 * Commit number since last tag.
 */
#define BUILD							"'$BUILD'"

/**
 * Git current number of commits for the given branch.
 */
#define TOTAL_NUM_COMMITS				"'$TOTAL_NUM_COMMITS'"

/**
 * Current git branch.
 */
#define BRANCH							"'$BRANCH'"

/**
 * How many commits away of master branch.
 */
#define AHEAD_BY_MASTER					"'$AHEAD_BY_MASTER'"

/**
 * How many untracked git files.
 */
#define TOTAL_NUM_UNTRACKED				"'$TOTAL_NUM_UNTRACKED'"

/**
 * Latest git commit hash.
 */
#define COMMIT_HASH						"'$COMMIT_HASH'"

/**
 * Latest git commit timestamp.
 */
#define COMMIT_TIMESTAMP				"%ci"

/**
 * Latest git commit date( Yy-Mm-Dd ).
 */
#define COMMIT_DATE						"'${COMMIT_DATE}'"

/**
 * Latest git commit time( Hh:Mm:Ss ).
 */
#define COMMIT_TIME						"'${COMMIT_TIME}'"

/**
 * Compile date( Yy-Mm-Dd ).
 */
#define COMPILE_DATE					"'${COMPILE_DATE}'"

/**
 * Compile time( Hh:Mm:Ss ).
 */
#define COMPILE_TIME					"'${COMPILE_TIME}'"

/**
 * Latest git commit timezone.
 */
#define COMMIT_TIMEZONE					"'${COMMIT_TIMEZONE}'"

/**
 * Latest git commit comment.
 */
#define COMMIT_COMMENT					"%s"

#endif /* GIT_VERSION_H_ONLY_ONE_INCLUDE_SAFETY */' --date=short --abbrev-commit > ${GITV_H_TMP} 2> /dev/null

# Replace the file only when the new file is different
if [ ! -f ${GITV_H} ]; then
    #File is not found. Make temporary the new header file
    mv ${GITV_H_TMP} ${GITV_H}
else
    #File is found so replace it only when the new file is different
    diff ${GITV_H} ${GITV_H_TMP} || mv ${GITV_H_TMP} ${GITV_H}
fi

#Remove temporary file
rm -rf ${GITV_H_TMP}

