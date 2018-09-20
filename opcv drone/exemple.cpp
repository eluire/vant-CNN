#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include<iostream>
#include<stdio.h>

using namespace cv;
using namespace std;
	// função para alterar o nome do vídeo a cada new recorder
	string intToString(int num){

		// obj do tipo stringstream criado
		std::stringstream ss;
		// obj recebeu numero 
		ss << num;
		//retornou o numero sendo alocado no final da string que solicitou a função
		return ss.str();
	}
	// inicio da implementação
	int main(int argc, char* argv[]){
		
		bool Gravando = false;
		bool iniciar_nova_gravacao = false;
		int cont=0;
		bool primeira_vez = true;

		// obj de captura de video criado!
		VideoCapture cap;
		// obj videowriter criado!
		VideoWriter NewVideoWriter;
		// abriu a conecxão com o IP do drone e capturou o fluxo de imgs  
		cap.open("tcp://192.168.1.1:5555");
		// chave para verificar se existe fluxo de video
		if (!cap.isOpened())
		{
			cout << "FAIL!!! nao foi capturado o fluxo de video" << endl;
			return 0;
		}
		//janela criada!
		namedWindow("Camera Drone",CV_WINDOW_AUTOSIZE);
		// alocação das dimenções do frame 
		double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
		double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

		cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

		//set framesize para ser usado pelo obj videowriter
		Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));


		while (1)
		{
			//se estamos iniciando uma nova gravação:
			if(iniciar_nova_gravacao==true)
			{

				// obj NewVideoWriter setado com os parâmetros descritos e salvo na pasta onde o script é executado
				NewVideoWriter  = VideoWriter("Video"+intToString(cont)+".avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true); 
			
				Gravando = true;
				iniciar_nova_gravacao = false;
				cout<<"Novo video com o nome: Video"+intToString(cont)+".avi "<<endl;


				if ( NewVideoWriter.isOpened() ) // chava para verificar se o obj está open
				{
					cout << "FAIL!!! obj VideoWriter nao executado" << endl;

					return 0;
				}

			}

			Mat frame;
			cap >> frame;// está passando o frame capturado para a matriz 

			//se estamos no modo gravando:
			if(Gravando == true)
			{
				// vai escrever o frame no arquivo de video 
				NewVideoWriter.write(frame);
				//show "REC" in top left corner in red
				//be sure to do this AFTER you write to the file so that "REC" doesn't show up
				//on the recorded video.
				putText(frame,"REC",Point(0,60),2,2,Scalar(0,0,255),2);
			}
			//mostrando a camera em uma janela 
			imshow("Camera Drone", frame); 

			//decisões tomadas a partir da teclas precionadas
			switch(waitKey(10))
			{
				case 70:// tecla p apertada (pause)
					
					Gravando =!Gravando;

					if(primeira_vez == true)
					{

						cout << "Nova gravação inicíada" << endl;

						if ( !NewVideoWriter.isOpened() ) 
						{
							cout << "FAIL!!! obj VideoWriter nao executado" << endl;
							return 0;
						}
						primeira_vez = false;
					}
					else 
					{
						if (!Gravando)
						{
							cout << "gravação parou" << endl;
						}
						else
						{ 
							cout << "gravação iniciada" << endl;
						}
					}
				break;

				case 72: //tecla r apertada(record)
					//'n' has been pressed
					//começando um novo arquivo de video 
					iniciar_nova_gravacao = true;
					cout << "nova gravação iniciada" << endl;
					//inrementando o contador 
					cont+=1;
				break;

				case 27:// tecla esc apertada
				
					cout << "Saindo" << endl;
					return 0;

			}

		}

	return 0;
	
	}

		