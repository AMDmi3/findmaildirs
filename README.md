# findmaildirs #

This is a very simple utility to be used with mutt to simplify automatic
mailboxes management.

It scans given directory for maildir-type mailboxes and prints list of
them in mutt-compatible format, so its output may be used for `mailboxes'
configuration directive of mutt.

## Usecase ##

For example, you have a bunch of maildirs:

```
~/.mail
    inbox
        cur
        new
        tmp
    sent
        cur
        new
        tmp
    maillists
        lkml
            cur
            new
            tmp
        freebsd
            announce
                cur
                new
                tmp
            ports
                cur
                new
                tmp
```

to make these available in mutt, you need following in your .muttrc:

```
set mbox_type=Maildir
set folder="~/.mail"

mailboxes +inbox +sent +maillists/lkml +maillists/freebsd/ports \
          +maildirs/freebsd/announce
```

it's a pain to manage manually, especially of you have many mailboxes
and even more painful if you have mailboxes created automatically
by e.g. procmail.

This little utility manages it for you so you can just use:

```
mailboxes `findmaildirs ~/.mail`
```

Though it's extremely simple task and you could use simple `find | sed`
pipeline instead, this utility acts a bit more clever and doesn't descend
into subdirectories of mailboxes (cur, new and tmp) which may noticeably
slow down mutt startup if there are many mail messages.

## Installation ##

No required library dependencies. Just run

```
make && make install
```

Makefile honours CC, CFLAGS, PREFIX (/usr/local by default) and DESTDIR.

## License ##

2-clause BSD, see COPYING.

## Author ##

* [Dmitry Marakasov](https://github.com/AMDmi3) <amdmi3@amdmi3.ru>
