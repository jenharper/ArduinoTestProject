﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Uduino;

public class MainSceneScript : MonoBehaviour
{

    private int counter = 0;
    public UduinoManager m_uduinoController;
    public float BrushAngle;
    private int PotentiometerReading = -1; //an invalid value
    private float desiredAngle = 0;
    private bool requestChange = false;

    private const float AngleOfZeroReading = 90;
    private const float AngleOf1023Reading = -90;

    public GameObject myBrushObject;

    void Awake()
    {
        m_uduinoController.OnDataReceived += OnDataReceived; //Create the Delegate
        m_uduinoController.alwaysRead = true; // This value should be On By Default
    }

    // Use this for initialization
    void Start()
    {
        Debug.Log("Bonjour!  MainSceneScript is alive.");
        //  m_uduinoController.DiscoverPorts();
    }

    //May want to move this to statemanage
    void OnDataReceived(string data, Uduino.UduinoDevice deviceName)
    {
        //    Debug.Log(deviceName.lastRead + " from " + deviceName.name  + " aka " + deviceName.identity + " said " + data);   

        if (deviceName.name == "SelfEmbodimentBrush")
        {
            int indexOfExclamation = data.IndexOf('!');
            if ((data.Length > 3) && (indexOfExclamation > 2))
            {
                //P: indicates a potentiometer reading
               
                if ((data[0] == 'P') && (data[1] == ':'))
                {
                    int temp;
                    string strippedString = data.Substring(2, indexOfExclamation-2);
                    bool readingOK = int.TryParse(strippedString, out temp);
                    if (readingOK && temp >= 0 && temp < 1024)
                    {
                        PotentiometerReading = temp;
                        // Debug.Log("Potentiometer value = " + PotentiometerReading);
                    }
                }
            }
            else
            {
                Debug.Log("Unknown message:" + data);
            }

        }
        else
        {
            Debug.Log("Message from unknown Arduino: " + deviceName.name + " on port " + deviceName.identity + ".");
        }

    }

    // Update is called once per frame
    void Update()
    {
        counter++;
        if (counter > 500)
        {
            counter = 0;
            Debug.Log("MainSceneScript heartbeat");
        }


        if ((myBrushObject != null) && (PotentiometerReading >= 0))
        {
            desiredAngle = AngleZeroTo360(GetBrushAngleFromPotentiometerReading());
            myBrushObject.transform.localEulerAngles = new Vector3(desiredAngle, myBrushObject.transform.localEulerAngles.y, myBrushObject.transform.localEulerAngles.z);
        }


        if (Input.GetKeyDown(KeyCode.B))
        {
            Debug.Log("Started brush sequence");
            m_uduinoController.sendCommand("brush", 1, 734);
           
        }
        if (Input.GetKeyDown(KeyCode.S))
        {
            Debug.Log("Resetting brush");
            m_uduinoController.sendCommand("brush", 0, 271);
        
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            Debug.Log("Releasing the motor");
            m_uduinoController.sendCommand("releaseMotor");
        
        }



    }

    private float GetBrushAngleFromPotentiometerReading()
    {
        float slope = (AngleOf1023Reading - AngleOfZeroReading) / 1023.0f; //rise/run

        return (slope * PotentiometerReading) + AngleOfZeroReading;
    }

    private float AngleZeroTo360(float inputAngle)
    {
        float temp = inputAngle;
        while (temp < 0)
            temp += 360;

        while (temp >= 360)
            temp -= 360;

        return temp;
    }
}
