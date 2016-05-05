#include "cabecera.h"

int main (int argc, char *argv[]) {
	
	char *servidor_ip;
	char *servidor_puerto;
	char respuesta[1024];
    const char* mensaje;
	struct sockaddr_in direccion;
	int s;
	int n, enviados, recibidos;
    string edificio = "";
	// Comprobar los argumentos y tomarlos
    int opcion = menu();
	switch(opcion){
		case (1):{ 
			string missatge = pedirEdificios();
			mensaje = const_cast<char*>(missatge.c_str());
			break; }
		case (2):{ 
			string missatge = pedirEstanciasOcupantesEdificio();
			mensaje = const_cast<char*>(missatge.c_str());
			break; }
		case (3):{
			string missatge = pedirEdificiosSinOcupantes();
			mensaje = const_cast<char*>(missatge.c_str());
			break; }
		case (4):{
            cin>>edificio;
            string missatge = pedirEstanciasOcupantesEdificio();
            mensaje = const_cast<char*>(missatge.c_str());
            break; }
		case (5):{
            string missatge = "hola";
            //mensaje = const_cast<char*>(missatge.c_str());
            mensaje = &missatge[0];
            break; }
		default: {
            string missatge = "hola";
            mensaje = const_cast<char*>(missatge.c_str());
            break; }
	}

	string ip = "172.25.224.194";
	servidor_ip = const_cast<char*>(ip.c_str());
	string puerto = "80";
	servidor_puerto = const_cast<char*>(puerto.c_str());

	
	printf("\n\rEnviar mensaje \"%s\" a %s:%s...\n\r\n\r", mensaje, servidor_ip, servidor_puerto);

	// Paso 1: Abrir el socket

	s = socket(AF_INET, SOCK_STREAM, 0); // Creo el socket
	if (s == -1)
	{
		fprintf(stderr, "Error. No se puede abrir el socket\n\r");
		return 1;
	}
	printf("Socket abierto\n\r");
    cout<<"Mensaje: "<<mensaje<<endl;
	// Paso 2: Conectar al servidor		

	// Cargar la direccion
	direccion.sin_family = AF_INET; // socket familia INET
	direccion.sin_addr.s_addr = inet_addr(servidor_ip);
	direccion.sin_port = htons(atoi(servidor_puerto));
	if (connect(s, (struct sockaddr *)&direccion, 	sizeof (direccion)) == -1)
	{
		fprintf(stderr, "Error. No se puede conectar al servidor\n\r");
		close(s);
		return 1;
	}
	printf("Conexion establecida\n\r");

	// Paso 3: Enviar mensaje

	n = strlen(mensaje);
	enviados = write(s, mensaje, n);
	if (enviados == -1 || enviados < n)
	{
		fprintf(stderr, "Error enviando el mensaje\n\r");
		close(s);
		return 1;
	}

	printf("Mensaje enviado\n\r");

	// Paso 4: Recibir respuesta

	n = sizeof(respuesta) - 1;
	recibidos = read(s, respuesta, n);
	if (recibidos == 1)
	{
		fprintf(stderr, "Error recibiendo respuesta\n\r");
		close(s);
		return 1;
	}
	string cuerpo = leerCuerpo(respuesta);
	cout<<respuesta<<endl;
	while(respuesta[recibidos-1]!='\n'){
		n = sizeof(respuesta) - 1;
		recibidos = read(s, respuesta, n);
		if (recibidos == 1)
		{
			fprintf(stderr, "Error recibiendo respuesta\n\r");
			close(s);
			return 1;
		}
		respuesta[recibidos] = '\0';
		cuerpo += respuesta;
		cout<<respuesta<<endl;
	}

	cuerpo = CorregirJSON(cuerpo);
	SepararJSON(cuerpo, opcion, edificio);


    //Crear objeto json
    //Guardar los valores en variables
    //Crear tablas
    
	// Paso 5: Cerrar el socket
	close(s);
	printf("Socket cerrado. Comunicacion finalizada\n\r");

	return 0;
}
