#!/bin/sh

text_file=data/20ng/text_train.txt # the text file for training
label_file=data/20ng/label_train.txt # the label file for training
infer_file=data/20ng/text_all.txt # the text file to infer
output_path=workspace/

window=5 # the window size for the construction of the word-word network
min_count=0 # discard words that appear less than <min_count>

# heterogeneous text network construction
./text2hin/data2w -text ${text_file} -output-ww ${output_path}ww.net -output-words ${output_path}words.node -window ${window} -min-count ${min_count}
./text2hin/data2dl -text ${text_file} -label ${label_file} -output-lw ${output_path}lw.net -output-labels ${output_path}labels.node -output-dw ${output_path}dw.net -output-docs ${output_path}docs.node -min-count ${min_count}

cat ${output_path}ww.net ${output_path}dw.net ${output_path}lw.net > ${output_path}text.hin
cat ${output_path}words.node ${output_path}docs.node ${output_path}labels.node > ${output_path}text.node

# learn predictive word representations
./pte/pte -nodes ${output_path}text.node -words ${output_path}words.node -hin ${output_path}text.hin -output ${output_path}word.emb -binary 1 -size 100 -negative 5 -samples 300 -threads 20

# infer the embeddings of the texts provided in the <infer_file>
./text2vec/infer -infer ${infer_file} -vector ${output_path}word.emb -output ${output_path}text.emb -debug 2 -binary 0
