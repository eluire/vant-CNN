#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include<iostream>
#include<stdio.h>

using namespace cv;
using namespace std;
//função para identificar o video com um número de ordem crescente de gravação
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main(int argc, char* argv[])
{
	bool recording = false;
	bool startNewRecording = false;
	int inc=0;
	bool firstRun = true;

	VideoCapture cap; //obj VideoCapture criado
	VideoWriter oVideoWriter;// obj videowriter criado e ainda não iniciado 

	cap.open("tcp://192.168.1.1:5555"); // capturar o fluxo de video disponibilizado na porta 5555 do drone
	if (!cap.isOpened())  // chave para verificar se o fluxo foi capturado corretamente
	{
		cout << "FAIL!!! nao foi capturado o fluxo de video" << endl;
		return -1;
	}

	namedWindow("Camera",CV_WINDOW_AUTOSIZE); //janela camera criada

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //otendo a largura dos frames do video 
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //otendo a largura dos frames do video 

	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

	//set framesize para usar como  videoWriter obj
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));


	while (1) {

		if(startNewRecording==true){

			
			oVideoWriter  = VideoWriter("Video"+intToString(inc)+".avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true); //inicializando VideoWriter obj 
			 

			recording = true;
			startNewRecording = false;
			cout<<"Novo arquivo Video"+intToString(inc)+".avi criado"<<endl;


			if ( !oVideoWriter.isOpened() ) //Se o obj videoWriter nao for inicializado corretamente ele sai da aplicação 
			{
				cout << "FAIL!! nao inicializou video writing obj" << endl;
				getchar();
				return 0;
			}

		}


		Mat frame;

		bool bSuccess = cap.read(frame); // lendo um novo filme do video

		if (!bSuccess) //chave para verificar se o frame foi capturado
		{
			cout << "FAIL!! frame não capturado" << endl;
			break;
		}

		//se estiver gravando escreva o frame 
		if(recording){

			oVideoWriter.write(frame);
			//show "REC" in top left corner in red
			//be sure to do this AFTER you write to the file so that "REC" doesn't show up
			//on the recorded video.
			putText(frame,"REC",Point(0,60),2,2,Scalar(0,0,255),2);


		}
		imshow("Camera drone", frame); //Mostrando o frame na jenela Camera drone 


		switch(waitKey(10)){

		case 112:
			//tecla p apertada (pause)
			recording =!recording;

			if(firstRun == true){

				cout << "Nova gravação iniciada" << endl;
				//oVideoWriter  = VideoWriter("Video0.avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);

				if ( !oVideoWriter.isOpened() ) 
			{
				cout << "FAIL!! nao inicializou video writing obj" << endl;
				getchar();
				return -1;
			}
				firstRun = false;


			}
			else {if (!recording)cout << "Gravação parada" << endl;

			else cout << "Gravação iniciada" << endl;
			}
			break;

		case 114:
			//'tecla r apertada (record)
			//start new video file
			startNewRecording = true;
			cout << "Nova gravação iniciada" << endl;
			//incrementando o nome do video
			inc+=1;
			break; 
		case 27:
			//botão 'esc' precionado (sair da aplicação)
			cout << "Sair" << endl;
			return 0;



		}


	}

	return 0;

}