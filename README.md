# PTE
This is an implementation of the PTE model for learning predictive text embeddings [(PTE: Predictive Text Embedding through Large-scale Heterogeneous Text Networks)](https://arxiv.org/abs/1508.00200). 

The codes consist of three parts. Codes in the "text2hin" folder are used to generate heterogeneous text networks from raw text data. We put the codes of the PTE model in the "pte" folder, which learns predictive word representations given a heterogeneous text network. After getting the word representations, the "text2vec" folder will be used to infer the embeddings of other text data, e.g., documents, sentences.

We also provide four datasets used in the PTE paper, which are the 20 newsgroups (20NG), IMDB (IMDB), DBLP titles (DBLP), moview review (MR) datasets. The data can be found in the "data" folder.

## Install
Our codes rely on two external packages, which are the Eigen package and the GSL package.

#### Eigen
The [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) package is used for matrix operations. To run our codes, users need to download the Eigen package and modify the package path in the makefile.

#### GSL
The [GSL](https://www.gnu.org/software/gsl/) package is used to generate random numbers. After installing the package, users also need to modify the package path in the makefile. 

## Compile
After installing the two packages and modifying the package paths, users may go to every folder and use the makefile to compile the codes.

## Data
The PTE model uses a set of labeled texts for training, therefore users need to provide a text file and a label file, which contain the tokens and labels of the given texts respectively. For the text file, the i-th line contains the tokens of the i-th text, and the tokens are separated by spaces. For the label file, the number of lines is the same of the text file, and the i-th line records the label of the i-th text. We provide four examples in the data folder.

After model training, users can use the learned word representations to infer the embeddings of other texts. To do that, users need to provide another text file, containing the tokens of the texts to infer. The file format is the same as the text file for modeling training, where each line contains a number of tokens separated by spaces. Users can also find several examples in the data folder.

## Running
To run the PTE model, users may directly use the example script (run.sh) we provide. In this script, we will first construct the heterogeneous text networks based on the training files. Then we will call the PTE model to learn predictive word representations, which will be further used to infer the embeddings of other text data.

## Contact: 
If you have any questions about the codes and data, please feel free to contact us.
```
Meng Qu, qumn123@gmail.com
Jian Tang, tangjianpku@gmail.com
```

## Citation
```
@inproceedings{tang2015pte,
title={Pte: Predictive text embedding through large-scale heterogeneous text networks},
author={Tang, Jian and Qu, Meng and Mei, Qiaozhu},
booktitle={Proceedings of the 21th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining},
pages={1165--1174},
year={2015},
organization={ACM}
}
```

