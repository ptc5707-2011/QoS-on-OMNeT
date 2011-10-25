estimate = function(trace_filename, output_filename, time_between_samples, predictions_ahead, input_samples, debug=FALSE, debug_number=10) {

    #Limpar wokspace
    #rm(list=ls(all=T))

    #Bibliotecas
    require(fracdiff)
    require(forecast)
    require(zoo)

    ########Parâmetros###############################################
    #trace_filename = './R_input/trace-dir1-2010-06-07-21-00-100us'
    #time_between_samples=100E-6

    #predictions_ahead=100
    #input_samples=1000

    ##DEBUG
    #debug=TRUE
    #debug_number=10
    plot_number_of_samples = input_samples  #Deve ser menor ou igual a input_samples
    #################################################################

    #Função auxiliar, faz a estimação do número de bytes que deve passar pela próxima janela
    predict_total_bytes = function(x,h) {
    		predictions = forecast(fracdiff(x),h=h)
    		return(sum(predictions$mean))
    }

    #Função auxiliar, calcula o o número de bytes que efetivamente passaram por uma janela
    calculate_total_bytes = function(x,h) {
    		return(sum(x[(length(x)-h+1):length(x)]))
    }

    #Leitura de amostras do arquivo
    if(debug) {
    	samples_read = scan(trace_filename, what=list(x='',y=0), skip=0,sep='\t', nlines=input_samples+debug_number*predictions_ahead)
    } else {
    	samples_read = scan(trace_filename, what=list(x='',y=0), skip=0,sep='\t')
    }

    samples = samples_read$y
    rm(samples_read)

    #Cálculo da estimação do número de bytes que passarão por uma janela de tamanho 
    #definido por <<predictions_ahead>>, usando <<input_samples>> amostras anteriores como entrada.
    total_bytes_prediction = as.vector(rollapply(zoo(samples),width=input_samples, FUN=predict_total_bytes, h=predictions_ahead, by=predictions_ahead, align="left"))

    #Se em modo de debug, plota predições (vermelho) e número de bytes "reais" (azul), para comparação
    if(debug) {
	
    	total_bytes_actual = as.vector(rollapply(zoo(samples),width=input_samples, FUN=calculate_total_bytes, h=predictions_ahead, by=predictions_ahead, align="left"))
    	total_bytes_actual = total_bytes_actual[2:length(total_bytes_actual)]
    	total_bytes_prediction = total_bytes_prediction[1:(length(total_bytes_prediction)-1)]
	
    	plot(total_bytes_actual, type='o', col='blue')
    	lines(total_bytes_prediction, type='o', col='red')
	
    }


    cat(round(total_bytes_prediction),file=output_filename, sep='\n')
	

}

