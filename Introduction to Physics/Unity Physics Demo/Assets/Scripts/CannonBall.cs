﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonBall : MonoBehaviour {

    public float forceOnFire = 300.0f;

    bool fire = false;
    bool canFire = true;

    Rigidbody rigidbody = null;
	
    void Awake()
    {
        rigidbody = GetComponent<Rigidbody>();
        rigidbody.isKinematic = true;
    }

	// Update is called once per frame
	void Update () {

        if (Input.anyKeyDown && canFire)
        {
            rigidbody.isKinematic = false;
            rigidbody.AddForce(transform.forward * forceOnFire);
            canFire = false;
        }
	}
}
