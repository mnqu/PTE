#!/bin/sh

text_file=data/20ng/text_train.txt
label_file=data/20ng/label_train.txt
infer_file=data/20ng/text_all.txt
output_path=workspace/

window=5
min_count=0

./text2hin/data2w -text ${text_file} -output-ww ${output_path}ww.net -output-words ${output_path}words.node -window ${window} -min-count ${min_count}
./text2hin/data2dl -text ${text_file} -label ${label_file} -output-lw ${output_path}lw.net -output-labels ${output_path}labels.node -output-dw ${output_path}dw.net -output-docs ${output_path}docs.node -min-count ${min_count}

cat ${output_path}ww.net ${output_path}dw.net ${output_path}lw.net > ${output_path}text.hin
cat ${output_path}words.node ${output_path}docs.node ${output_path}labels.node > ${output_path}text.node

./pte/pte -nodes ${output_path}text.node -words ${output_path}words.node -hin ${output_path}text.hin -output ${output_path}word.emb -binary 1 -size 100 -negative 5 -samples 300 -threads 20

./text2vec/infer -infer ${infer_file} -vector ${output_path}word.emb -output ${output_path}text.emb -debug 2 -binary 0
