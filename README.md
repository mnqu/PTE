#PTE
This is an implementation of the PTE model for learning predictive text embeddings [(PTE: Predictive Text Embedding through Large-scale Heterogeneous Text Networks)](https://arxiv.org/abs/1508.00200). 
Our codes consist of three parts. Codes in the "text2hin" folder are used to generate heterogeneous text networks from raw text data. We put the codes of the PTE model in the "pte" folder, which learn predictive word representations given a heterogeneous text network. After getting the word representations, we will use the "text2vec" folder to infer the embeddings of some given texts, e.g., documents, sentences.
We also provide four datasets used in the PTE paper, which are the 20 newsgroups (20NG), IMDB (IMDB), DBLP titles (DBLP), moview review (MR) datasets. The data can be found in the "data" folder.

##Install
Our codes rely on two external packages, which are the Eigen package and GSL package.

####Eigen
The [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) package is used for matrix manipulation. To run our codes, users need to download the Eigen package and modify the package path in the makefile.

####GSL
The [GSL](https://www.gnu.org/software/gsl/) package is used to generate random numbers. After installing the package, users may need to modify the path of the GSL package in the makefile. 

##Compile
After installing the two packages and modifying the package paths, users may go to every folder and use the makefile to compile the codes.

##Data
The PTE model uses a set of labeled texts for training, therefore uses need to provide a text file and a label file, which contain the tokens and labels of the given texts respectively. For the text file, the i-th line contains the tokens of the i-th text, and the tokens are separated by spaces. For the label file, the number of lines is the same of the text file, and the i-th line records the label of the i-th text. We provide four examples in the data folder.

After model training, users can use the learned word representations to infer the embeddings of unseen texts. To do that, users need to provide another text file, containing the tokens of the texts to infer. The input format is the same as the text file for modeling training, where each line contains a number of tokens separated by spaces. Users can also find several examples in the data folder.

##Running
To run our codes, users may directly modify the training script (run.sh) we provide. During training, the script will first construct the heterogeneous text networks based on the training files. Then the script will call the PTE model for training. Finally, it will infer the embeddings of the texts for inferences.

##Contact: 
```
Meng Qu, qumn123@gmail.com
Jian Tang, tangjianpku@gmail.com
```

##Citation
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

