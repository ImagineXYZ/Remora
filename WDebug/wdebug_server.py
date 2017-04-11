import sys
import socket
import threading
import time
import datetime

QUIT = False

def getIP():
    hostname = socket.gethostname()
    IP = socket.gethostbyname(hostname)
    print('-----------------------')
    print('My IP is: '+IP)
    print('-----------------------')
    return IP

class ClientThread( threading.Thread ):
    ''' 
    Class that implements the client threads in this server
    '''

    def __init__( self, client_sock ):
        '''
        Initialize the object, save the socket that this thread will use.
        '''

        threading.Thread.__init__( self )
        self.client = client_sock

    def run( self ):
        ''' 
        Thread's main loop. Once this function returns, the thread is finished 
        and dies. 
        '''  
        done = False
        print("Ready")
        while not done:
            msg = self.readbytes().decode('utf-8')
            if("WD_EOT" in msg):
                done = True
            print(msg)
        print("Socket Closed")
                
        #
        # Make sure the socket is closed once we're done with it
        #
        
        return

    def readbytes( self ):
        ''' 
        Helper function, reads up to 1024 chars from the socket, and returns 
        them as a string
        '''

        result = self.client.recv(512)
        return result

    def writeline( self, text ):
        ''' 
        Helper function, writes teh given string to the socket, with an end of 
        line marker appended at the end 
        '''
        self.client.send(text.encode('utf-8'))



            
#Clase servidor que acepta multiples clientes.
class Server(threading.Thread):
    #Método de inicialización.
    def __init__( self ):
        self.sock = None
        self.thread_list = []
        threading.Thread.__init__( self )
    #Método principal que se encarga de aceptar solicitudes de clientes.
    def run( self ):
        try:#Se intenta creación de servidor 
            self.sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
            #Se define conexión de socket hacia la IP propia del dispositivo y el puerto 9999
            self.sock.bind( ("192.168.137.1", 6660) )
            #Default IP Windows  192.168.137.1
            #Default IP Android  192.168.43.1

            
            #Se define timeout de 5 segundos para que el cliente responda.
            self.sock.listen( 5 )
        except socket.error as err:
            print ('Socket connection error...')
            #Si no responde se elimina el socket perteneciente al cliente.
            del self.sock

        try:
            while not QUIT:
                try:
                    # Se define tiempo de respuesta para el cliente.
                    self.sock.settimeout( 0.500 )
                    #Se acepta conexión proveniente del cliente.
                    client = self.sock.accept()[0]
                except socket.timeout:
                    #Si no se recibe nada, se termina la ejecución.
                    time.sleep( 1 )
                    if QUIT:
                        print ("Received quit command. Shutting down...")
                        break
                    continue
                #Creación de hilo de ejecución para cliente entrante.
                new_thread = ClientThread( client )
                print ('Incoming Connection. Started thread ')
                
##                print (new_thread.getName())
                #Se agrega cliente a lista de hilos.
                self.thread_list.append( new_thread )
                #Se inicializa 
                new_thread.start()
                for thread in self.thread_list:
                    if not thread.isAlive():
                        self.thread_list.remove( thread )
                        thread.join()

        except Exception as err:
            print ('Exception caught: %s\nClosing...' % err)

        self.sock.close()
        
server = Server()
server.start()
