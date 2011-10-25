source('estimate.R')

arquivos=list(
'trace-dir0-2010-05-23-10-00-100us',
'trace-dir0-2010-05-23-15-00-100us',
'trace-dir0-2010-05-23-21-00-100us',
'trace-dir0-2010-05-30-10-00-100us',
'trace-dir0-2010-05-30-15-00-100us',
'trace-dir0-2010-05-30-21-00-100us',
'trace-dir0-2010-06-07-10-00-100us',
'trace-dir0-2010-06-07-15-00-100us',
'trace-dir0-2010-06-07-21-00-100us',
'trace-dir1-2010-05-23-10-00-100us',
'trace-dir1-2010-05-23-15-00-100us',
'trace-dir1-2010-05-23-21-00-100us',
'trace-dir1-2010-05-30-10-00-100us',
'trace-dir1-2010-05-30-15-00-100us',
'trace-dir1-2010-05-30-21-00-100us',
'trace-dir1-2010-06-07-10-00-100us',
'trace-dir1-2010-06-07-15-00-100us'
)

analises = list(
c(100,10),
c(100,100),
c(1000,10),
c(1000,100),
c(1000,1000)
)

for(arquivo in arquivos) {
	caminho_arquivo = paste('R_input/traces/',arquivo,sep='')
	print(caminho_arquivo)
	for(analise in analises) {
		caminho_output = paste('R_output/analises/',arquivo,'_in',analise[1],'_out',analise[2],sep='')
		estimate(caminho_arquivo,caminho_output,100E-6,analise[2],analise[1])
		print(paste(caminho_output,'done'))
	}
}