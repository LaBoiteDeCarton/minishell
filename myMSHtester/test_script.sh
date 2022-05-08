#!/bin/bash

MSH_PATH="../"
RESET="\033[0m"
GREEN="\033[32m"
RED="\033[31m"
BOLDRED="\033[1m\033[31m"
CYAN="\033[36m"
WHITE="\033[37m"
BOLDYELLOW="\033[1m\033[33m"
BOLDWHITE="\033[1m\033[37m"
INT32_MAX=2147483647
INT64_MAX=9223372036854775807

mkdir SpaceDir
cd SpaceDir
>test_script_ERR

function exec_test()
{
	printf $BOLDRED
	echo $@ | bash >bash_res 2>bash_err;
	BASH_ES=$?;
	echo $@ | ../"$MSH_PATH"minishell >msh_res 2>msh_err
	MSH_ES=$?;
	printf $WHITE"COMMAND:$CYAN $@\n"$RESET
	printf $BOLDWHITE"std_out | std_err | \$?\n"
	diff msh_res bash_res >/dev/null
	if [ $? -eq 0 ]; then
		printf "    $GREEN%s$RESET   " "✓ "
	else
		printf "    $RED%s$RESET   " "✗ "
		echo "COMMAND: $@" >>test_script_ERR;
		echo "STDOUT ERROR :" >>test_script_ERR; 
		echo "BASH OUTPUT :" >>test_script_ERR
		cat -e bash_res >>test_script_ERR
		echo "MSH OUTPUT :" >>test_script_ERR
		cat -e msh_res >>test_script_ERR
		echo "" >>test_script_ERR;
	fi
	diff msh_err bash_err >/dev/null
	if [ $? -eq 0 ]; then
		printf "    $GREEN%s$RESET    " "✓ "
	else
		printf "    $RED%s$RESET   " "✗ "
		echo "COMMAND: $@" >>test_script_ERR;
		echo "STDERR ERROR :" >>test_script_ERR;
		echo "BASH ERROR OUTPUT :" >>test_script_ERR
		cat -e bash_err >>test_script_ERR
		echo "MSH ERROR OUTPUT :" >>test_script_ERR
		cat -e msh_err >>test_script_ERR 
		echo "" >>test_script_ERR;
	fi
	if [ $BASH_ES -eq $MSH_ES ]; then
		printf " $GREEN%s$RESET " "✓ "
	else
		printf " $RED%s$RESET " "✗ "
		echo "COMMAND: $@" >>test_script_ERR;
		echo "EXIT_STATUS ERROR :" >>test_script_ERR;
		echo  "msh:($MSH_ES) bash:($BASH_ES)" >> test_script_ERR;
		echo "" >> test_script_ERR;
	fi
	printf "\n$RESET$WHITE"
}

exec_test " "
exec_test "\\n"
#ECHO TEST
#echo with one argument
exec_test "echo salut"
#echo with one argument in double/single scope
exec_test "echo \"salut\""
exec_test "echo 'salut'"
#echo with 2 argument
exec_test "echo espace espace"
#echo with 2 arguments testing the space beetween argument in STDOUT
exec_test "echo kjhkjh \" 	klkjj\""
exec_test "echo kjhkjh   \"klkjj\""
exec_test "echo kjhkjh  	  \"klk      jj\""
exec_test "echo \"    kjhkjh     \"  	  \"	klk      jj\""
#echo with one argument and double/single scope in it
exec_test "echo hkjhkjh\"hjk\""
exec_test "echo 't'est"
#echo with -n option
exec_test "echo -n salut le monde"
exec_test "echo -n -n -n -n salut le monde"
exec_test "echo -nnnnnnnnnnn salut le monde avec des n"


#BUILTIN EXIT
#exit with differents number arguments
exec_test "exit"
exec_test "exit 1"
exec_test "exit $INT32_MAX"
exec_test "exit $INT64_MAX"
exec_test "exit 9223372036854775808"
exec_test "exit -1"
exec_test "exit -$INT32_MAX"
exec_test "exit -$INT64_MAX"
exec_test "exit -92233720368547758078"
#exit with errors in argument
exec_test "exit 123 456"
exec_test "exit 123 tropcesttrop"
exec_test "exit pasbon"
exec_test "exit pasbon pas bon du tout"
exec_test "exit pas bon 123 456"
exec_test "exit 42sh"
#exit before creating the new file
exec_test "exit 15 && touch NEVER"
if [ -e NEVER ]; then
	printf $BOLDRED"FILE \"NEVER\" WAS CREATED THIS IS NOT WHAT WE EXCPECTED SO... %s$RESET\n" "✗"
fi
rm NEVER 2> /dev/null
#exit before heredoc but should trigger the heredoc


#TEST VARIABLES (On assume que && fonctionne si && KO ici ce sera largment KO)
exec_test "echo \$NEXISTEPAS"
exec_test "echo \"\$NEXISTEPAS\""
exec_test "echo '\$NEXISTEPAS'"
exec_test "echo \"bonjour \$NEXISTEPAS le monde\""
exec_test "echo 'bonjour \$NEXISTEPAS le monde'"
exec_test "echo  	 	salut  	 	 \$NEXISTEPAS  	 	  toi"
exec_test "EXISTE=quelquechose && echo \$EXISTE"
exec_test "EXISTE=raté && echo \$EXISTEOUPAS"
exec_test "EXISTE=raté && echo \$EXISTE1"
exec_test "EXISTE=raté && echo \$NEXISTE"
exec_test "EXISTE=raté && echo \$ EXISTE"
exec_test "EXISTE=quelquechose && echo \$EXISTE\$OUPAS"
exec_test "EXISTE=quelquechose && echo \$EXISTE\$EXISTE"
exec_test "EXISTE=quelquechose && echo \$EXISTE\"\$EXISTE\""
exec_test "EXISTE=quelquechose && echo \$EXISTE'\$EXISTE'"
exec_test "EXISTE=quelquechose && echo \"\$EXISTE\""
exec_test "EXISTE=quelquechose && echo '\$EXISTE'"
#creation a var with $othervar in the content
exec_test "ARG=42 && ARG2=\"Vive \$ARG\" && echo \$ARG2"
exec_test "VAR=BOOM && BLABLA=\$VAR && echo \$BLABLA"
exec_test "VAR=BOOM && BLABLA=\"coucou \$VAR\"&&echo \$BLABLA"
exec_test "VAR=BOOM && BLABLA=\$VAR && VAR=FLOP && echo \$BLABLA && echo \$VAR"
exec_test "VAR=1 BIGVAR=2 OTHERVAR=3 && echo \$VAR \$BIGVAR \$OTHERVAR"
exec_test "VAR=4 BIGVAR=5 OTHERVAR=6 echo test && echo \$VAR \$BIGVAR \$OTHERVAR"
exec_test "VAR=1 BIGVAR=\$VAR OTHERVAR=3 && echo \$VAR \$BIGVAR \$OTHERVAR"
exec_test "VAR=\$NOTEXISTYET NOTEXISTYET=5 BIGVAR=\$VAR OTHERVAR=3 && echo \$VAR \$BIGVAR \$OTHERVAR"
exec_test "arg=something && echo '$'arg"
exec_test "arg=something && echo '$'\"arg\""
#trying to creat a var with $var in the name of var
exec_test "ARG=arg && \$ARG=OK && echo \$arg && echo \$ARG"

#TEST CMD le && doit fonctionner
printf "ceci sera le contenu du fichier\ndlekjelekj\naiop\naaaaaaa\nlkjlkjljlj\n\n\nkdjlksjdkldjaljkjblkjlkc\nkjlkjlkjbkjlkj\nkjkkjkjhhkhbc\n\nkjhkjhkjbbbbb\nbbbbbccbbccbbcbbc\ncbcbbcbcbcbcbc\nbbbbb\nbbbb\nbbb" >filetest
exec_test "cat filetest"
exec_test "nimp"
exec_test "nimp avec argument de nimp"
exec_test "cat nexistepas"
exec_test "cat nexistepas filetest"
exec_test "cat filetest nexistepas"
exec_test "ps"
exec_test "cat -e filetest"
exec_test "l\"s\""
exec_test "(ls)"
exec_test "(ls -l)"
exec_test "(ls) -l"
exec_test "'l'\"s\""
exec_test "ARG=ls && \$ARG"
exec_test "ARG=-l && ls \$ARG"
exec_test "ARG=\"echo dommage\" \$ARG"

#TEST REDIRECTIONS
exec_test "<filetest cat"
echo "toto et tata" >filetest2;
exec_test "<filetest cat filetest2"
rm filetest2 2> /dev/null
exec_test "< filetest cat"
exec_test "<\"filetest\" cat"
exec_test "< 	 	\"filetest\" cat"
exec_test "<'filetest' cat"
exec_test "<nexistepas cat"
exec_test "< nexistepas cat"
exec_test "<nexistepas echo echec"
exec_test "<     nexistepas echo echec"
exec_test "<\"<\""
exec_test "<filetest echo reussi"
exec_test "<		filetest		   echo  	reussi"
exec_test "<nexistepas <filetest cat"
exec_test "<nexistepas      <filetest cat"
exec_test "<filetest <nexistepas cat"
exec_test "<filetest cat >newfile && cat newfile"
rm newfile newfile2 2>/dev/null
exec_test "<filetest cat >newfile >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest cat >newfile >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test "<filetest cat >newfile >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test "<filetest cat >newfile >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test "<nexistepas cat >newfile >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test "<nexistepas cat >newfile >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test ">newfile <filetest cat >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "there is something here" > newfile
exec_test ">newfile <filetest cat >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest cat >    newfile   	 >			newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest cat >    newfile   	 >			newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<nexistepas cat >newfile >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<nexistepas cat >newfile >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest <nexistepas cat >newfile >newfile2 && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest <nexistepas cat >newfile >newfile2 || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test "<filetest nimp"
exec_test "<nexistepas nimp"
touch noaccess
chmod 000 noaccess
exec_test "<noaccess cat"
exec_test "<filetest cat >noaccess"
exec_test "<filetest cat >noaccess >newfile"
rm newfile 2> /dev/null
exec_test "<filetest cat >newfile >noaccess && cat newfile"
rm newfile 2> /dev/null
exec_test "<filetest cat >newfile >noaccess || cat newfile"
rm newfile 2> /dev/null
#Ordre d'ouverture des redirections
printf "ceci sera le contenu du fichier\ndlekjelekj\naiop\naaaaaaa\nlkjlkjljlj\n\n\nkdjlksjdkldjaljkjblkjlkc\nkjlkjlkjbkjlkj\nkjkkjkjhhkhbc\n\nkjhkjhkjbbbbb\nbbbbbccbbccbbcbbc\ncbcbbcbcbcbcbc\nbbbbb\nbbbb\nbbb" >filetest
exec_test ">newfile <filetest cat && cat newfile"
rm newfile 2> /dev/null
printf "ceci sera le contenu du fichier\ndlekjelekj\naiop\naaaaaaa\nlkjlkjljlj\n\n\nkdjlksjdkldjaljkjblkjlkc\nkjlkjlkjbkjlkj\nkjkkjkjhhkhbc\n\nkjhkjhkjbbbbb\nbbbbbccbbccbbcbbc\ncbcbbcbcbcbcbc\nbbbbb\nbbbb\nbbb" >filetest
exec_test ">newfile <filetest cat || cat newfile"
rm newfile 2> /dev/null
printf "ceci sera le contenu du fichier\ndlekjelekj\naiop\naaaaaaa\nlkjlkjljlj\n\n\nkdjlksjdkldjaljkjblkjlkc\nkjlkjlkjbkjlkj\nkjkkjkjhhkhbc\n\nkjhkjhkjbbbbb\nbbbbbccbbccbbcbbc\ncbcbbcbcbcbcbc\nbbbbb\nbbbb\nbbb" >filetest
exec_test ">newfile cat <filetest && cat newfile"
rm newfile 2> /dev/null
printf "ceci sera le contenu du fichier\ndlekjelekj\naiop\naaaaaaa\nlkjlkjljlj\n\n\nkdjlksjdkldjaljkjblkjlkc\nkjlkjlkjbkjlkj\nkjkkjkjhhkhbc\n\nkjhkjhkjbbbbb\nbbbbbccbbccbbcbbc\ncbcbbcbcbcbcbc\nbbbbb\nbbbb\nbbb" >filetest
exec_test ">newfile cat <filetest || cat newfile"
rm newfile 2> /dev/null
exec_test ">newfile >noaccess <filetest >newfile2 cat && cat newfile newfile2"
rm newfile newfile2 2>/dev/null
exec_test ">newfile >noaccess <filetest >newfile2 cat || cat newfile newfile2"
rm newfile newfile2 2>/dev/null
echo "toto et tata" >filetest2;
exec_test "<filetest <filetest2 cat"
rm filetest2 2>/dev/null
exec_test "<nexistepas ls && ls >newfile || cat newfile"
rm newfile 2>/dev/null
#Arguments et redirections
exec_test "<filetest FARG=memo >\$FARG"
if [ ! -e 'memo' ]; then
	printf $BOLDRED"FILE \"memo\" WAS NOT CREATED THIS IS NOT WHAT WE EXCPECTED SO... %s$RESET\n" "✗"
fi
if [ -e '$FARG' ]; then
	printf $BOLDRED"FILE \"\$FARG\" WAS CREATED THIS IS NOT WHAT WE EXCPECTED SO... %s$RESET\n" "✗"
fi
rm '$FARG' 2> /dev/null
rm memo 2>/dev/null

#TEST ||
exec_test "<nexistepas ARG=89 || echo \$ARG"
exec_test "ARG=ECHEC nimp ||  echo \$ARG"
#TEST PIPE

exec_test "echo test | echo | hkhjhk | echo && echo reussi"
exec_test "echo test | echo | hkhjhk | kjhkhkj && echo dommage"


#TEST &&
exec_test "< filetest grep a && < nexistepas cat"
exec_test "< filetest grep a && echo YOU WIN"
exec_test "< nexistepas grep a && < nexistepasnonplus cat"
exec_test "< nexistepas grep a && echo YOU FAILED"
rm filetest 2> /dev/null
#TEST PARANTHESIS
echo "do not sigquit please, wait 8seconds"
exec_test "(sleep 4 && echo premier fini >> file) | (sleep 2 && echo deuxieme fini >>file)"
diff file ../filecmp >/dev/null
if [ ! $? -eq 0 ]; then
	printf $BOLDRED"FILE CONTENT IS NOT WHAT EXPECTED... %s$RESET\n" "✗"
fi

exec_test "< | >"

chmod 777 noaccess
rm noaccess
cd ..
cp SpaceDir/test_script_ERR test_script_ERR
rm -rf SpaceDir

#TEST ||

exit