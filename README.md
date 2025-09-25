~~~bash
newgrp docker
~~~

Build docker
~~~bash
docker build -t cpp-dev -f Dockerfile.docker .
~~~

Run your container
~~~bash
docker run -it --rm -v $(pwd):/workspace -w /workspace cpp-dev bash
~~~

Run C++ file
~~~bash
g++ -std=c++20 main.cpp -o main
./main
~~~


