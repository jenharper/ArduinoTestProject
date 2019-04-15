using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Uduino;

public class MainSceneScript : MonoBehaviour {

    private int counter = 0;
    public UduinoManager m_uduinoController;
    public float BrushAngle;
    private int PotentiometerReading;

    void Awake()
    {
        m_uduinoController.OnDataReceived += OnDataReceived; //Create the Delegate
        m_uduinoController.alwaysRead = true; // This value should be On By Default
    }

    // Use this for initialization
    void Start () {
        Debug.Log("Bonjour!  MainSceneScript is alive.");
      //  m_uduinoController.DiscoverPorts();
    }

    //May want to move this to statemanage
    void OnDataReceived(string data, Uduino.UduinoDevice deviceName)
    {
        //    Debug.Log(deviceName.lastRead + " from " + deviceName.name  + " aka " + deviceName.identity + " said " + data);   

        if (deviceName.name == "myBrushArduino")
        {
            int temp;
            int indexOfExclamation = data.IndexOf('!');
            string strippedString = data.Substring(0, indexOfExclamation);
            bool readingOK = int.TryParse(strippedString, out temp);
            if (readingOK && temp >= 0 && temp < 1024)
            {
                PotentiometerReading = temp;
               // Debug.Log("Potentiometer value = " + PotentiometerReading);
            }
            else
            {
                Debug.Log("Failed to parse value. " + strippedString);
            }

        }
        else
        {
            Debug.Log("Message from unknown Arduino" + deviceName.name);
        }
           
    }

    // Update is called once per frame
    void Update () {
        counter++;
        if (counter > 500)
        {
            counter = 0;
            Debug.Log("MainSceneScript heartbeat");
        }
	}
}
