CC = gcc
GCCPARAMS = -Wall -O2

project1dependencies = src/project1.o\
					   src/analex.o\
					   src/common.o

project2dependencies = src/project2.o\
					   src/anasynt.o\
					   src/common.o

project3dependencies = src/project3.o\
					   src/find.o\
					   src/anasynt.o\
					   src/global.o\
					   src/processeur.o\
					   src/common.o		

project4dependencies = src/project4.o\
					   src/compilation.o\
					   src/find.o\
					   src/global.o\
					   src/processeur.o\
					   src/analex.o\
					   src/anasynt.o\
					   src/executeur.o\
					   src/interpret.o\
					   src/common.o		

project5dependencies = src/project5.o\
					   src/compilation.o\
					   src/find.o\
					   src/global.o\
					   src/processeur.o\
					   src/analex.o\
					   src/anasynt.o\
					   src/common.o		

project6dependencies = src/project6.o\
					   src/compilation.o\
					   src/find.o\
					   src/global.o\
					   src/processeur.o\
					   src/analex.o\
					   src/anasynt.o\
					   src/executeur.o\
					   src/common.o	

compiledependencies =  src/compile.o\
					   src/compilation.o\
					   src/find.o\
					   src/global.o\
					   src/processeur.o\
					   src/analex.o\
					   src/anasynt.o\
					   src/file.o\
					   src/common.o

runtimedependencies =  src/executeur.o\
					   src/global.o\
					   src/anasynt.o\
					   src/runtime.o\
					   src/processeur.o\
					   src/file.o\
					   src/common.o

src/%.o: src/%.c
	${CC} ${GCCPARAMS} -o $@ $< -c

project1: ${project1dependencies}
	${CC} -o $@ ${project1dependencies} ${GCCPARAMS}

project2: ${project2dependencies}
	${CC} -o $@ ${project2dependencies} ${GCCPARAMS}

project3: ${project3dependencies}
	${CC} -o $@ ${project3dependencies} ${GCCPARAMS}

project4: ${project4dependencies}
	${CC} -o $@ ${project4dependencies} ${GCCPARAMS}

project5: ${project5dependencies}
	${CC} -o $@ ${project5dependencies} ${GCCPARAMS}

project6: ${project6dependencies}
	${CC} -o $@ ${project6dependencies} ${GCCPARAMS}

lacbuild: ${compiledependencies}
	${CC} -o $@ ${compiledependencies} ${GCCPARAMS}

lacrun: ${runtimedependencies}
	${CC} -o $@ ${runtimedependencies} ${GCCPARAMS}

.PHONY: clean
clean:
	rm -rf src/*.o project* lacbuild lacrun 