CC?=		gcc
CFLAGS+=	-Wall -Wextra -pedantic

PREFIX?=	/usr/local

PROGRAM=	findmaildirs
SOURCES=	findmaildirs.c

TESTDIR=	testdir

# for compatibility with FreeBSD ports
BSD_INSTALL_PROGRAM?=	install -m 555

all: ${PROGRAM}

${PROGRAM}: ${SOURCES}
	${CC} ${CFLAGS} ${SOURCES} -o ${PROGRAM}

clean: cleantest
	rm -f ${PROGRAM}

install: ${PROGRAM}
	mkdir -p ${DESTDIR}${PREFIX}/bin
	${BSD_INSTALL_PROGRAM} ${PROGRAM} ${DESTDIR}${PREFIX}/bin/

createtest:
	@mkdir -p ${TESTDIR}/case1/box1/new
	@mkdir -p ${TESTDIR}/case1/box1/cur
	@mkdir -p ${TESTDIR}/case1/box1/tmp
	@mkdir -p ${TESTDIR}/case1/box2/new
	@mkdir -p ${TESTDIR}/case1/box2/cur
	@mkdir -p ${TESTDIR}/case1/box2/tmp
	@mkdir -p ${TESTDIR}/case1/folder1/box3/new
	@mkdir -p ${TESTDIR}/case1/folder1/box3/cur
	@mkdir -p ${TESTDIR}/case1/folder1/box3/tmp
	@mkdir -p ${TESTDIR}/case1/folder1/box4/new
	@mkdir -p ${TESTDIR}/case1/folder1/box4/cur
	@mkdir -p ${TESTDIR}/case1/folder1/box4/tmp
	@mkdir -p ${TESTDIR}/case1/folder2/box5/new
	@mkdir -p ${TESTDIR}/case1/folder2/box5/cur
	@mkdir -p ${TESTDIR}/case1/folder2/box5/tmp
	@mkdir -p ${TESTDIR}/case2/box1/new
	@mkdir -p ${TESTDIR}/case2/box1/cur
	@mkdir -p ${TESTDIR}/case2/box1/tmp
	@mkdir -p ${TESTDIR}/case2/box2/new
	@mkdir -p ${TESTDIR}/case2/box2/cur
	@mkdir -p ${TESTDIR}/case2/box2/tmp
	@mkdir -p ${TESTDIR}/case2/box2/box3/new
	@mkdir -p ${TESTDIR}/case2/box2/box3/cur
	@mkdir -p ${TESTDIR}/case2/box2/box3/tmp
	@mkdir -p ${TESTDIR}/case2/box2/box4/new
	@mkdir -p ${TESTDIR}/case2/box2/box4/cur
	@mkdir -p ${TESTDIR}/case2/box2/box4/tmp
	@mkdir -p ${TESTDIR}/case2/box2/box4/box5/new
	@mkdir -p ${TESTDIR}/case2/box2/box4/box5/cur
	@mkdir -p ${TESTDIR}/case2/box2/box4/box5/tmp

cleantest:
	rm -rf ${TESTDIR}

test: createtest ${PROGRAM}
	test "$$(./${PROGRAM} ${TESTDIR}/case1 | tr ' ' '\n' | env LC_ALL=C sort | xargs echo)" \
		= "+box1 +box2 +folder1/box3 +folder1/box4 +folder2/box5"
	test "$$(./${PROGRAM} ${TESTDIR}/case2 | tr ' ' '\n' | env LC_ALL=C sort | xargs echo)" \
		= "+box1 +box2 +box2/box3 +box2/box4 +box2/box4/box5"
