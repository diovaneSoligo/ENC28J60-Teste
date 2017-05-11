#include <SPI.h>
#include <UIPEthernet.h>

byte mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };//endereço mac

IPAddress ip(192,168,0,6);//seta o ip fixo
EthernetServer server(80); //porta do servidor, pois ele vai ser o server

void setup()
{
  //iniciar o servidor, passa o mac e o ip fixo, da pra passar só mac, que ai vai te dar um ip por DHCP
  // deve seguir a sequencia Ethernet.begin(mac, ip, dns, gateway, subnet);
  Ethernet.begin(mac, ip);
  server.begin();

  //visualizar info da placa
  Serial.begin(9600);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Mascara : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS-ServerIP: ");
  Serial.println(Ethernet.dnsServerIP());
}

void loop() 
{
  // verifica as conexões recebidas
  EthernetClient client = server.available();
  long time = millis();
  
  if (client) // se tiver cliente conectado
  {
    boolean currentLineIsBlank = true;
    
    while (client.connected()) // em quanto o cliente estiver conectado
    {
      if (client.available())
      {
        char c = client.read();

        // Recebeu dois caracteres de fim de linha (\ n). O cliente terminou
        // envio do pedido.
        if (c == '\n' && currentLineIsBlank) 
        {
          // Enviar um cabeçalho de resposta HTTP padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // quando a pagina carregar por total fechar conexão
          client.println("Refresh: 2");  // recarregar a cada 2 segundos
          client.println();
          
          // envia o html
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>Ethernet ENC28J60</title></head>");
          client.println("<body>");
          client.println("<h1>Hello World! <br> Se vc esta vendo esta pagina eh porque o ENC28j60 esta funcionando :D </h1>");

          // Tempo de funcionamento do servidor
          client.println("<p>Servidor esta ligado a: ");
          client.println(time);
          client.println(" ms.</p><br><br><h6>Diovane Soligo</h6>");
          client.println("</body></html>"); // fim do html
          
          break; // fim da transmissão
        }
        
        // Se o cliente enviou um caractere de fim de linha (\ n),
        // eleve a bandeira e veja se o próximo caractere é outro
        // caractere de fim de linha
        if (c == '\n')
        {
          currentLineIsBlank = true;
        } 
        
        // Se o cliente envia um caractere que não é um caractere de fim de linha,
        // redefinir o sinalizador e aguarde o caractere de fim de linha.
        else if (c != '\r')
        {
          currentLineIsBlank = false;
        }
      }
    }
    // tempo pro navegador receber os dados
    delay(500);
    
    // fexa conexão
    client.stop();
  }
}
