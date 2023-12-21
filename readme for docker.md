#run these in root directory


#build Ubuntu

docker build -t vsdk-1.8.2-on-ubuntu -f Dockerfile-Ubuntu/Dockerfile .
docker run -it --rm vsdk-1.8.2-on-ubuntu

#list all images
docker images -a

#remove images
docker rmi 


