#!/ bin / sh

#-- -- -- -- -- -- -- -- - COLOR -- -- -- -- -- -- -- -- - #
# 0 - No style | 1 - Bold
RED = "\e[0;31m" BRED = "\e[1;31m" GRN = "\e[0;32m" BGRN = "\e[1;32m" YEL =
    "\e[0;33m" BYEL = "\e[1;33m" BLU = "\e[0;34m" BBLU = "\e[1;34m" PUR =
        "\e[0;35m" BPUR = "\e[1;35m" CYA = "\e[0;36m" BCYA = "\e[1;36m" WHI =
            "\e[0;37m" BWHI = "\e[1;37m" GRE = "\e[2;37m"

#-- -- -- -- -- -- -- -- - GLOBV -- -- -- -- -- -- -- -- - #
    ref_out = / tmp / ref_out.out my_out = / tmp /
                                           my_out.out

                                           simple_echo() {
  local name = $1 echo - e "${PUR} $name ${NC}" echo "echo exited with 0!" >
               "$ref_out" echo $
      ? >> "$ref_out"./ myredir "foo.txt"
                                "echo"
                                "foo"
                                "bar"
                                "test" &> "$my_out" echo $
      ? >> "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

not_a_command() {
  local name = $1 echo - e "${PUR} $name ${NC}" echo
                           "myredir: patatras: command not found" >
               "$ref_out" echo "1" >> "$ref_out"./ myredir
                                          "test.txt"
                                          "patatras" &> "$my_out" echo "$?" >>
      "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

no_argument() {
  local name = $1 echo - e "${PUR} $name ${NC}" echo "2" >
               "$ref_out"./ myredir &> "$my_out" echo $
      ? > "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

missing_command() {
  local name =
      $1 echo - e "${PUR} $name ${NC}" echo "myredir: missing some argument" >
      "$ref_out" echo "2" >> "$ref_out"./ myredir "test.txt" &> "$my_out" echo
                                                                "$?" >>
      "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

ls_here() {
  local name = $1 echo - e "${PUR} $name ${NC}" echo "ls exited with 0!" >
               "$ref_out" echo "$?" >> "$ref_out"./ myredir
                                           "test.txt"
                                           "ls" &> "$my_out" echo "$?" >>
      "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

ls_nonexistent() {
  local name = $1 echo - e "${PUR} $name ${NC}" echo
                           "ls: cannot access 'nonexistent': No such file or "
                           "directory\nls exited with 2!" >
               "$ref_out" echo "$?" >>
               "$ref_out"./ myredir "test.txt"
                                    "ls"
                                    "nonexistent" &> "$my_out" echo "$?" >>
      "$my_out" if["$(diff " $ref_out " " $my_out ")" == ""];
  then echo - e "${GRN}PASSED${NC}" else echo - e "${RED}NOT PASSED${NC}" echo -
      e ${RED} EXPECTED : $BRED$(cat $ref_out) $WHI echo -
                          e ${RED} GOT : $BRED$(cat $my_out) $WHI fi
}

simple_echo "simple echo" not_a_command "not a command" no_argument
            "no argument" missing_command "missing command" ls_here
            "ls here" ls_nonexistent "ls nonexistent"

    exit 0;
