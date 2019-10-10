
/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.
*/
/* This header allows for sharing of the two functions among internal
 * and external fuse builds. */

int drop_privs(void);
int restore_privs(void);

#ifdef __SOLARIS__
/*
 * Solaris doesn't have setfsuid/setfsgid.
 * This doesn't really matter anyway as this program shouldn't be made
 * suid on Solaris. It should instead be used via a profile with the
 * sys_mount privilege.
 */

int drop_privs(void)
{
    return (0);
}

int restore_privs(void)
{
    return (0);
}

#else /* __SOLARIS__ */
int drop_privs(void)
{
	if (!getegid()) {

		gid_t new_gid = getgid();

		if (setresgid(-1, new_gid, getegid()) < 0) {
			perror("priv drop: setresgid failed");
			return -1;
		}
		if (getegid() != new_gid){
			perror("dropping group privilege failed");
			return -1;
		}
	}

	if (!geteuid()) {

		uid_t new_uid = getuid();

		if (setresuid(-1, new_uid, geteuid()) < 0) {
			perror("priv drop: setresuid failed");
			return -1;
		}
		if (geteuid() != new_uid){
			perror("dropping user privilege failed");
			return -1;
		}
	}

	return 0;
}

int restore_privs(void)
{
	if (geteuid()) {

		uid_t ruid, euid, suid;

		if (getresuid(&ruid, &euid, &suid) < 0) {
			perror("priv restore: getresuid failed");
			return -1;
		}
		if (setresuid(-1, suid, -1) < 0) {
			perror("priv restore: setresuid failed");
			return -1;
		}
		if (geteuid() != suid) {
			perror("restoring privilege failed");
			return -1;
		}
	}

	if (getegid()) {

		gid_t rgid, egid, sgid;

		if (getresgid(&rgid, &egid, &sgid) < 0) {
			perror("priv restore: getresgid failed");
			return -1;
		}
		if (setresgid(-1, sgid, -1) < 0) {
			perror("priv restore: setresgid failed");
			return -1;
		}
		if (getegid() != sgid){
			perror("restoring group privilege failed");
			return -1;
		}
	}

	return 0;
}
#endif /* __SOLARIS__ */
