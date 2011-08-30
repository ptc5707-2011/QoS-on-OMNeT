#ATENÇÃO!!!!!!: Antes de rodar este script, usar o comando setwd() para configurar o caminho da pasta R_src de dentro do workspace atual. Exemplo:
#setwd("~/workspace/QoS/R_src")
#em seguida:
#source("calc_qos.R")

#Limpar o workspace
rm(list=ls(all=T))

###########################
#######PARÂMETROS##########
#ATENÇÃO!!!!!: Todos os caminhos de arquivos a seguir devem ser mudados para corresponder com o nome dos arquivos que você exportou do OMNeT. Aqui, por exemplo, eu escolhi chamar a saída da simulação CBR-File, do delay do nó R1 como CBR-File-5s-1.csv, da flag de received_or_dropped do nó R1 como CBR-File-5s-2.csv, assim por diante. 
# NÃO ESPERE QUE O SCRIPT VÁ FUNCIONAR SE VOCÊ NÃO ADEQUAR O NOME DOS ARQUIVOS PARA O SEU CASO.

sent_timestamp_filename =		"./R_input/CBR-File-R1-5s-1.csv"
delay_filename =				"./R_input/CBR-File-R1-5s-2.csv"
length_filename =				"./R_input/CBR-File-R1-5s-3.csv"
seq_filename =					"./R_input/CBR-File-R1-5s-4.csv"
received_or_dropped_filename =	"./R_input/CBR-File-R1-5s-5.csv"

###########################
###########################

#Ler os arquivos (usando scan ao invés de read.csv, read.csv é muito lento)
sent_timestamp = scan(sent_timestamp_filename, what=list(x=0,y=0), skip=1,sep=',')
delay = scan(delay_filename, what=list(x=0,y=0), skip=1,sep=',')
length = scan(length_filename, what=list(x=0,y=0), skip=1,sep=',')
seq = scan(seq_filename, what=list(x=0,y=0), skip=1,sep=',')
received_or_dropped = scan(received_or_dropped_filename, what=list(x=0,y=0), skip=1,sep=',')

#Agregar os vetores em um único data frame
result = data.frame(sent_timestamp$y, sent_timestamp$x, seq$y, received_or_dropped$y, delay$y, length$y)
#Nomear as colunas do data frame
colnames(result) = c("sent time", "arrival time", "sequence number", "received (1) or dropped (0)", "delay", "length")

##Agregação dos resultados
##########################
#Substituir tempos de envio quando houve erro por NA
result[[1]][result[[5]]==0] = NA

#Substituir tempos de chegada quando houve erro por NA
result[[2]][result[[5]]==0] = NA

#Substituir 0 no delay por NA
result[[5]][result[[5]]==0] = NA



#Criar grupos para agregação
N = dim(result)[1]
if(N%%10) {
	G = N
} else {
	G = N+(10-N%%10)	
}
group = rep(1:G,each=10)
group = group[1:N]

#Cálculo do atraso médio, ignorando NA
avg_delay = tapply(result[[5]], group, mean, na.rm=T)

#Cálculo do throughput
throughput = tapply(result[[6]], group, sum)

#Cálculo da variação do atraso
delay_variation = c(0,avg_delay[2:dim(avg_delay)]-avg_delay[1:(dim(avg_delay)-1)])

#Cálculo da taxa de erro de pacotes
packet_error_rate = 1-tapply(result[[4]], group, mean)

#Criação de um data frame com todos os dados
aggregated = data.frame(seq(from=1,by=1,length.out=dim(avg_delay)), avg_delay,delay_variation, throughput,packet_error_rate)
colnames(aggregated)=c("time window (s)", "average delay (s)", "delay_variation (s)", "throughput (bytes/s)", "packet_error_rate(%)")

#PLOTS
#ATENÇÃO!!!!: SUBSTITUIR A FUNÇÃO quartz() por funções apropriadas em Windows ou Linux para abrir uma nova janela.
plot(result[[1]], main="sent time")
quartz()
plot(result[[2]], main="arrival time")
quartz()
plot(result[[4]], main="received (1) or dropped (0)")
quartz()
plot(result[[5]], main="delay")
quartz()
plot(result[[6]], main="length")