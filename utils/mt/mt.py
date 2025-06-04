#!/usr/bin/env python3
"""
mt - Multi-Tool
a simple project-aware tool for cais-rl.
written by: @Tarek-Sallam
"""

import os
import sys
import subprocess
from pathlib import Path
import json


class MT:
    """Simple project control tool."""
    
    def __init__(self):
        self.repo_root = self._get_repo_root()
        self.projects = self._load_projects_config()
    
    # get the projects from the projects.yml file
    def _load_projects(self):
        project_config_path = Path(__file__).parent / 'projects.json'
        with open(project_config_path, 'r') as f:
            config = json.load(f)
            return config.get('projects', {})
    
    # get the root of the git repository
    def _get_repo_root(self):
        current_dir = Path.cwd()
        while current_dir != current_dir.parent:
            if (current_dir / '.git').exists():
                return current_dir
            current_dir = current_dir.parent
        raise RuntimeError("Not in a git repository")
    
    # get the project that the user is in
    def which_project(self):
        cwd = Path.cwd().resolve()
        repo = self.repo_root.resolve()
        
        if not str(cwd).startswith(str(repo)):
            return None
        
        rel_path = str(cwd.relative_to(repo))
        
        for project, path in self.projects.items():
            if rel_path.startswith(path):
                return project
    
    # get the path of the project root
    def get_project_path(self, project):
        if project not in self.projects:
            return None
        return self.repo_root / self.projects[project]
    
    # run a command in the project
    def run_in_project(self, project, command_args):
        project_root = self.get_project_path(project)
        if not project_root or not project_root.exists():
            print(f"❌ Project '{project}' not found")
            return 1
        
        print(f"🎯 [{project}] Running: {' '.join(command_args)}")
        result = subprocess.run(command_args, cwd=project_root)
        return result.returncode
    
    def status(self):
        """Show current status."""
        current = self.detect_project()
        if current:
            print(f"📍 Current project: {current}")
        else:
            print("📍 Not in any project")
        
        print("\n📋 Available projects:")
        for name, path in self.projects.items():
            exists = "✅" if (self.repo_root / path).exists() else "❌"
            print(f"   {exists} {name}")
