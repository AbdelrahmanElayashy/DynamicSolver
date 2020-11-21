while getopts m:t:n: flag
do
  case "${flag}" in 
    m) model=${OPTARG};;
    t) tests=${OPTARG};;
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
    echo -e "\e[92mThe result will be in ${name_folder}";
     echo -e "\e[39m///////////////////////////////////////////";
      echo "///////////////////////////////////////////"
else
   echo "///////////////////////////////////////////";
    echo "///////////////////////////////////////////"
    echo -e "\e[92mYour choose for model : Lotka-volterra ${model}";
    echo -e "\e[92mThe result will be in ${name_folder}";
     echo -e "\e[39m///////////////////////////////////////////";
      echo "///////////////////////////////////////////";
fi

sleep 5

cp draw_execution_time.py ./$name_folder
cp calculate_model_error.py ./$name_folder

cd $name_folder
cmake ..
make
./program $model $tests



echo "Finish"
