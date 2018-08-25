#! /bin/sh

# Copyright © 2017 Aleksey Cherepanov <lyosha@openwall.com>
# Redistribution and use in source and binary forms, with or without
# modification, are permitted.

# Compile chmod_nofollow.c and put there (or change the path)
chmod_nofollow=/home/workerpublic/bin/chmod_nofollow.elf
delay=1

while :; do
    # -maxdepth 2000 is an arbitrary limitation to avoid DoS.
    # TODO: it would be better to catch it and report.
    # TODO: check permissions in find?
    find /home/share/ -maxdepth 2000 -execdir "$chmod_nofollow" {} +
    sleep "$delay"
done
