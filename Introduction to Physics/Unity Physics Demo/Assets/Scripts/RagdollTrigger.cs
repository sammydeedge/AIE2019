﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RagdollTrigger : MonoBehaviour {

	void OnTriggerEnter(Collider other)
    {
        Ragdoll ragdoll = other.gameObject.GetComponentInParent<Ragdoll>();
        if (ragdoll != null)
        {
            Debug.Log("Character has entered Trigger Zone");
            ragdoll.RagdollOn = true;
        }
    }
}