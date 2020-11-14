while getopts m:a:n: flag
do
  case "${flag}" in 
    m) model=${OPTARG};;
    a) accuracy=${OPTARG};;
    n) name_folder=${OPTARG};;
  esac
done


if [ -d "$name_folder" ];then 
  echo "ERROR! : choose another folder name";
  exit 1;
else
  mkdir $name_folder
fi

if [ ${model} -eq 0 ];then
    echo "///////////////////////////////////////////";
     echo "///////////////////////////////////////////";
    echo -e "\e[92mYour choose for model : single track model";
    echo -e "\e[92mYour choose for acc. : 10^-${accuracy}";
    echo -e "\e[92mThe result will be in ${name_folder}";
     echo -e "\e[39m///////////////////////////////////////////";
      echo "///////////////////////////////////////////"
else
   echo "///////////////////////////////////////////";
    echo "///////////////////////////////////////////"
    echo -e "\e[92mYour choose for model : Lotka-volterra ${model}";
    echo -e "\e[92mYour choose for acc. : 10^-${accuracy}";
    echo -e "\e[92mThe result will be in ${name_folder}";
     echo -e "\e[39m///////////////////////////////////////////";
      echo "///////////////////////////////////////////";
fi

sleep 5

cp calc_error.py ./$name_folder
cd $name_folder
cmake ..
make
./program $model $accuracy


name_result="system_result_"
scheduler_result="resultScheduler.txt"
seq_result="resultSequRK4.txt"
search_folder=""

cd ..
for i in ${name_folder}/*; do
if [ -d $i ];then
  len=$((${#name_folder}+1))
  if [ ${i:len:${#name_result}} == ${name_result} ];then
    search_folder=${i:len:${#i}}  
  fi
fi
done

cd ${name_folder}
if [ -e ${scheduler_result} ];then
  mv `pwd`/${scheduler_result} `pwd`/${search_folder}/
fi
if [ -e ${seq_result} ];then
  mv `pwd`/${seq_result} `pwd`/${search_folder}/
fi


echo "Finish!"