graph=''
iterations=1000
nodes=60
prob_creation=0.5
prob_deletion=0.5
del_mean=5
del_variance=5

compile:
	g++ exercise.cpp adjacencylist.cpp graph.cpp queue.cpp -o analysis
	g++ markovian.cpp graph.cpp -o markovian
clean:
	rm *.txt
	rm *.png

plot:
	gnuplot plot.gb

analyze:
	make compile
	./analysis $(graph)

generate_randomly:
	make compile
	./markovian $(graph) $(iterations) $(nodes) $(prob_creation) $(prob_deletion) $(del_mean) $(del_variance)

help:
	@echo "Available commands:"
	@echo ""
	@echo "make compile --> compiles exercise.cpp and markovian.cpp providing 2 executables: analysis and markovian."
	@echo "The first one analyse the input graph returning the size and all the files to compute plots." 
	@echo "The second one, generates random dynamic graphs (exercise + proposal)  according to the given parameters."
	@echo ""
	@echo "make clean --> remove all .txt and .png files"
	@echo ""
	@echo "make analyze graph=<graph_name> --> start the analysis of the given graph, generating several txt files to plot graphs"
	@echo ""
	@echo "make generate_randomly graph=<destination_graph_name> iterations=<number_iterations> nodes=<number_nodes> prob_creation=<probability to create a link> prob_deletion=<probability to delete a link> del_mean=<deletion mean for the gaussian> del_variance=<deletion variance for the gaussian> --> generate 2 dynamic graphs"
	@echo "One using 2 fixed probabilities (prob_creation and prob_deletion)"
	@echo "The other one is the proposal which uses prob_creation to compute the probability to create links (together with degree of the nodes of the link) and a gaussian distribution to compute the life of the link (deletion"
	@echo ""
	@echo "make plot --> plot the files obtained from 'make analysis'"



