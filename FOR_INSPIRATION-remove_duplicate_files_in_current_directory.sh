#!/bin/sh

find "/home/laptop/backup-sony_xa1/apps/" -type f | sort > /tmp/apk_file_list.txt

find "/home/laptop/backup-sony_xa1/apps/" -type f -exec sh -c 'sha256sum "$1"' bash "{}" \; | sort --key=2 | cut --delimiter=' ' --fields=1 > /tmp/apk_checksums.txt


paste --delimiter=';' "/tmp/apk_file_list.txt" "/tmp/apk_checksums.txt" > "/tmp/apk_file_list_with_checksums.txt"


while IFS= read -r line
do
  apk_name="$(echo ${line} | cut --delimiter=';' --fields=1)"
  apk_checksum="$(echo ${line} | cut --delimiter=';' --fields=2)"
done < "/tmp/apk_file_list_with_checksums.txt"

