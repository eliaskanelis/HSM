#!/usr/bin/env bash

# This script should be run only from the Makefile
# It generates version.h

set -euo pipefail

GITV_H=inc/version.h
GITV_H_TMP=inc/version.h.tmp
mkdir -p "inc"

if git tag > /dev/null 2>&1; then
	# Repository exists
	GIT_RAW_VERSION=$(git describe --always --dirty --long --tags)
	TOTAL_NUM_COMMITS=$(git rev-list --count HEAD)
	BRANCH=$(git rev-parse --abbrev-ref HEAD)
	AHEAD_BY_MASTER=$(git rev-list --not master | wc -l | bc)
	TOTAL_NUM_UNTRACKED=$(git ls-files --exclude-standard --others --full-name -- . | wc -l | bc)
	COMMIT_HASH=$(git rev-parse --short --verify HEAD)

	IFS='-' read -r -a git_raw_version <<< "${GIT_RAW_VERSION}"
	IFS='v' read -r -a tag <<< "${git_raw_version[0]}"
	IFS='.' read -r -a major_minor <<< "${tag[1]}"

	MAJOR=${major_minor[0]}
	MINOR=${major_minor[1]}
	PATCH=${git_raw_version[1]}
	VERSION="$MAJOR.$MINOR.$PATCH"

	GIT_TIMESTAMP=$(git log -1 --pretty=format:'%ci' --date=short --abbrev-commit)
	IFS=' ' read -r -a timestamp <<< "${GIT_TIMESTAMP}"
	COMMIT_DATE=${timestamp[0]}
	COMMIT_TIME=${timestamp[1]}
	COMMIT_TIMEZONE=${timestamp[2]}
	COMMIT_TIMESTAMP=$(git log -1 --pretty=format:'%ci' --date=short --abbrev-commit)
	COMMIT_COMMENT=$(git log -1 --pretty=format:'%s' --date=short --abbrev-commit)
else
	# No repository here
	GIT_RAW_VERSION=""
	VERSION=""
	MAJOR=""
	MINOR=""
	PATCH=""
	TOTAL_NUM_COMMITS=""
	BRANCH=""
	AHEAD_BY_MASTER=""
	TOTAL_NUM_UNTRACKED=""
	COMMIT_HASH=""
	COMMIT_TIMESTAMP=""
	COMMIT_DATE=""
	COMMIT_TIME=""
	COMMIT_TIMEZONE=""
	COMMIT_COMMENT=""
fi

#Generate temporary header file
cat << EOF > ${GITV_H_TMP} 2> /dev/null
// ############################################################################
// ############################################################################
// About

/**
 * \\file     version.h
 *
 * \\brief    This file is generated automatically from script and keeps track
 *           of version control.
 *
 * Created:  ${COMMIT_DATE}
 *
 * \\author   Kanelis Elias    e.kanelis@voidbuffer.com
 */

/**
 * \\defgroup    Version    Version
 *
 * \code
 * #include "version.h"
 * \endcode
 */

// ############################################################################
// ############################################################################
// Code

#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

// ############################################################################
// ############################################################################
// Definitions

/**
 * Raw git version.
 */
#define GIT_RAW_VERSION                           "${GIT_RAW_VERSION}"

/**
 * Version of the project.
 */
#define VERSION                                   "${VERSION}"

/**
 * Major version from git tag.
 */
#define MAJOR                                     "${MAJOR}"

/**
 * Minor version from git tag
 */
#define MINOR                                     "${MINOR}"

/**
 * Commit number since last tag.
 */
#define PATCH                                     "${PATCH}"

/**
 * Git current number of commits for the given branch.
 */
#define TOTAL_NUM_COMMITS                         "${TOTAL_NUM_COMMITS}"

/**
 * Current git branch.
 */
#define BRANCH                                    "${BRANCH}"

/**
 * How many commits away of master branch.
 */
#define AHEAD_BY_MASTER                           "${AHEAD_BY_MASTER}"

/**
 * How many untracked git files.
 */
#define TOTAL_NUM_UNTRACKED                       "${TOTAL_NUM_UNTRACKED}"

/**
 * Latest git commit hash.
 */
#define COMMIT_HASH                               "${COMMIT_HASH}"

/**
 * Latest git commit timestamp.
 */
#define COMMIT_TIMESTAMP                          "${COMMIT_TIMESTAMP}"

/**
 * Latest git commit date( Yy-Mm-Dd ).
 */
#define COMMIT_DATE                               "${COMMIT_DATE}"

/**
 * Latest git commit time( Hh:Mm:Ss ).
 */
#define COMMIT_TIME                               "${COMMIT_TIME}"

/**
 * Latest git commit timezone.
 */
#define COMMIT_TIMEZONE                           "${COMMIT_TIMEZONE}"

/**
 * Latest git commit comment.
 */
#define COMMIT_COMMENT                            "${COMMIT_COMMENT}"

#endif /* VERSION_H_INCLUDED */
EOF

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
