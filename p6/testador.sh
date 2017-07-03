######################################
# Realiza automáticamente os teste   #
# do susy automáticamente      		 #
# Inspirado temáticamente no testador#
# do zanoni                          #
# A diferença desse é que ele faz o  #
# download dos arquivos do susy      #
######################################

#!/bin/bash
ESC_SEQ="\x1b["
COL_RED=$ESC_SEQ"31;01m"
COL_GREEN=$ESC_SEQ"32;01m"
COL_YELLOW=$ESC_SEQ"33;01m"
COL_BLUE=$ESC_SEQ"34;01m"
COL_MAGENTA=$ESC_SEQ"35;01m"
COL_CYAN=$ESC_SEQ"36;01m"
COL_BOLD=$ESC_SEQ";1m"


turma="mc558a"
lab="p6"

echo $turma

make
mkdir  testes

for i in {0..9}
do
    
	#if ! [ -f testes/arq$i.res ]; then
#		cd testes
#        curl -O -k http://www.ic.unicamp.br/~ra137105/mc558-2017-1/$lab/dados/arq$i.in
#        curl -O -k http://www.ic.unicamp.br/~ra137105/mc558-2017-1/$lab/dados/arq$i.res
#		cd ..
#	fi
#
	name=arq$i
#
	./lab < testes/$name.in > testes/$name.out
	echo $i
	diff -q testes/$name.out testes/$name.res &>/dev/null
#
	if [ $? -eq 0 ]; then
		printf "${COL_GREEN}%-12s${COL_RESET}\n" "OK"
	else
		printf "${COL_RED}%-12s${COL_RESET}\n" "Erro"
		echo ">>> Saida esperada (SuSy):"
		cat testes/arq$i.res
		echo ">>> Saida do seu programa:"
		cat testes/$name.out
		echo
		erros=$(($erros+1))
	fi
	echo
done

make clean
