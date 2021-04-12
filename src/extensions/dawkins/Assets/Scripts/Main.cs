using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[System.Serializable]
public class Configuration
{
    public int speed;
    public int population_size;
    public string initialModels;

    public static Configuration CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<Configuration>(jsonString);
    }
}

public class Main : MonoBehaviour
{
    public Text text;
    // Start is called before the first frame update
    void Start()
    {
        var config = Configuration.CreateFromJSON("{\"speed\":2, \"population_size\":50,\"initial_models\":\"random\"}");
        Debug.Log(config.speed);
        Debug.Log(config.population_size);
        Debug.Log(config.initialModels);
        // {"speed":2,"population_size":50,"initial_models":"random"}

        // var arguments = System.Environment.GetCommandLineArgs();
        // if (arguments.Length == 2)
        //     text.text = arguments[1];
        // else
        //     text.text = "Hey now brown cow!";
    }

    // Update is called once per frame
    void Update()
    {

    }
}
